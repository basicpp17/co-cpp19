// MIT License
// [[
//     Copyright 2026 HicknHack Sotware GmbH
//
//     Permission is hereby granted, free of charge, to any person obtaining a copy
//     of this software and associated documentation files (the "Software"), to deal
//     in the Software without restriction, including without limitation the rights
//     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//     copies of the Software, and to permit persons to whom the Software is
//     furnished to do so, subject to the following conditions:
//
//     The above copyright notice and this permission notice shall be included in all
//     copies or substantial portions of the Software.
//
//     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//     SOFTWARE.
// ]]
var Environment = require("qbs.Environment");
var File = require("qbs.File");
var FileInfo = require("qbs.FileInfo");
var Process = require("qbs.Process");
var TextFile = require("qbs.TextFile");

/**
 * Parse a URI string into its components.
 * Supports:
 *   - gh:user/repo@version#tag
 *   - gl:user/repo@version#tag
 *   - bb:user/repo@version#tag
 *   - https://github.com/user/repo.git@version#tag
 *   - https://example.com/package.zip@version
 *
 * @param {string} uri - The URI to parse
 * @returns {object} Parsed URI components: { scheme, user, repo, version, tag, url, type }
 */
function parseUri(uri) {
    var result = {
        scheme: null,
        user: null,
        repo: null,
        version: null,
        tag: null,
        url: null,
        type: "git", // "git", "archive", or "local"
        localPath: null
    };

    if (!uri || uri.trim() === "")
        return result;

    // Check for file:// scheme (local path)
    var fileMatch = uri.match(/^file:\/\/(.+)$/);
    if (fileMatch) {
        result.type = "local";
        result.localPath = fileMatch[1];
        result.url = uri;
        return result;
    }

    // Check for absolute local path (Windows or Unix)
    if (uri.match(/^[A-Za-z]:[\\/]/) || uri.match(/^\/[^\/]/)) {
        result.type = "local";
        result.localPath = uri;
        result.url = uri;
        return result;
    }

    // Extract version (@...) - only the last occurrence
    var versionMatch = uri.match(/@([^@#]+)(?:#|$)/);
    if (versionMatch) {
        result.version = versionMatch[1];
        uri = uri.replace(/@[^@#]+(?=#|$)/, "");
    }

    // Extract tag (#...) - only the last occurrence
    var tagMatch = uri.match(/#([^#]+)$/);
    if (tagMatch) {
        result.tag = tagMatch[1];
        uri = uri.replace(/#[^#]+$/, "");
    }

    // Check for shorthand schemes
    var shorthandMatch = uri.match(/^(gh|gl|bb):(.+)$/);
    if (shorthandMatch) {
        result.scheme = shorthandMatch[1];
        var path = shorthandMatch[2];
        var parts = path.split("/");
        if (parts.length >= 2) {
            result.user = parts[0];
            result.repo = parts.slice(1).join("/");
        }

        // Convert to full URL
        switch (result.scheme) {
        case "gh":
            result.url = "https://github.com/" + result.user + "/" + result.repo + ".git";
            break;
        case "gl":
            result.url = "https://gitlab.com/" + result.user + "/" + result.repo + ".git";
            break;
        case "bb":
            result.url = "https://bitbucket.org/" + result.user + "/" + result.repo + ".git";
            break;
        }
        result.type = "git";
    } else if (uri.match(/\.git\/?$/)) {
        // Full git URL
        result.url = uri;
        result.type = "git";

        // Try to extract user/repo from URL
        var gitMatch = uri.match(/([^\/]+)\/([^\/]+?)(?:\.git)?\/?$/);
        if (gitMatch) {
            result.user = gitMatch[1];
            result.repo = gitMatch[2].replace(/\.git$/, "");
        }
    } else if (uri.match(/\.(zip|tar\.gz|tar\.bz2|tar\.xz|tgz)(?:\?|$)/i)) {
        // Archive URL
        result.url = uri;
        result.type = "archive";

        // Try to extract name from URL
        var archiveMatch = uri.match(/([^\/]+)\.(zip|tar\.gz|tar\.bz2|tar\.xz|tgz)/i);
        if (archiveMatch) {
            result.repo = archiveMatch[1];
        }
    } else {
        // Assume it's a git URL without .git extension
        result.url = uri;
        result.type = "git";
    }

    // Default tag to version if not specified
    if (!result.tag && result.version) {
        if (result.version.match(/^[0-9a-f]{40}$/i)) {
            result.tag = result.version;
        } else {
            result.tag = "v" + result.version;
        }
    }

    return result;
}

/**
 * Get the cache directory for a package
 */
function getCacheDir(sourceCache, packageName, uriInfo) {
    var cacheBase = sourceCache;
    if (!cacheBase || cacheBase.trim() === "") {
        cacheBase = Environment.getEnv("QBS_SOURCE_CACHE");
    }
    if (!cacheBase || cacheBase.trim() === "") {
        // Default to temp directory
        cacheBase = FileInfo.joinPaths(Environment.getEnv("TEMP") || "/tmp", "qbs-source-cache");
    }

    var packageDir = packageName.replace(/\./g, "/");
    var versionDir = uriInfo.version || uriInfo.tag || "latest";

    return FileInfo.joinPaths(cacheBase, packageDir, versionDir);
}

/**
 * Clone a git repository
 */
function cloneGitRepo(url, tag, destDir) {
    console.info("OnlineSource: Cloning " + url + (tag ? " (tag: " + tag + ")" : ""));

    var process = new Process();
    try {
        var args = ["clone"];

        // Use shallow clone for tags
        if (tag) {
            args.push("--depth", "1");
            if (tag.match(/^[0-9a-f]{40}$/i)) {
                args.push("--revision", tag);
            } else {
                args.push("--branch", tag);
            }
        }

        args.push(url, destDir);

        process.exec("git", args, true);

        if (process.exitCode() !== 0) {
            throw "Git clone failed: " + process.readStdErr();
        }

        console.info("OnlineSource: Successfully cloned to " + destDir);
        return true;
    } finally {
        process.close();
    }
}

/**
 * Download and extract an archive
 */
function downloadArchive(url, destDir) {
    console.info("OnlineSource: Downloading " + url);

    File.makePath(destDir);

    var filename = url.match(/([^\/\?]+)(?:\?.*)?$/)[1];
    var archivePath = FileInfo.joinPaths(destDir, filename);

    var process = new Process();
    try {
        // Try curl first, fall back to wget
        var result = process.exec("curl", ["-L", "-o", archivePath, url], false);
        if (result !== 0) {
            result = process.exec("wget", ["-O", archivePath, url], false);
        }

        if (result !== 0) {
            throw "Failed to download archive: " + url;
        }

        // Extract based on extension
        if (filename.match(/\.zip$/i)) {
            process.exec("unzip", ["-o", archivePath, "-d", destDir], true);
        } else if (filename.match(/\.(tar\.gz|tgz)$/i)) {
            process.exec("tar", ["-xzf", archivePath, "-C", destDir], true);
        } else if (filename.match(/\.tar\.bz2$/i)) {
            process.exec("tar", ["-xjf", archivePath, "-C", destDir], true);
        } else if (filename.match(/\.tar\.xz$/i)) {
            process.exec("tar", ["-xJf", archivePath, "-C", destDir], true);
        }

        console.info("OnlineSource: Successfully extracted to " + destDir);
        return true;
    } finally {
        process.close();
    }
}

/**
 * Find Qbs project file in source directory
 * Returns the path to the main .qbs file, or null if not found
 */
function detectQbsProject(sourceDir, projectFile) {
    // If user specified a project file, use that
    if (projectFile) {
        var specifiedPath = FileInfo.joinPaths(sourceDir, projectFile);
        if (File.exists(specifiedPath)) {
            return {
                projectFile: specifiedPath,
                isQbsProject: true
            };
        }
        console.warn("OnlineSource: Specified project file not found: " + specifiedPath);
        return null;
    }

    // Look for .qbs files in root directory
    var entries = File.directoryEntries(sourceDir, File.Files);
    var qbsFiles = [];
    for (var i = 0; i < entries.length; i++) {
        if (entries[i].match(/\.qbs$/)) {
            qbsFiles.push(entries[i]);
        }
    }

    if (qbsFiles.length === 0) {
        return null;
    }

    if (qbsFiles.length === 1) {
        return {
            projectFile: FileInfo.joinPaths(sourceDir, qbsFiles[0]),
            isQbsProject: true
        };
    }

    // Multiple .qbs files - look for common patterns
    var preferredNames = ["project.qbs", sourceDir.split("/").pop() + ".qbs"];
    for (var j = 0; j < preferredNames.length; j++) {
        if (qbsFiles.indexOf(preferredNames[j]) !== -1) {
            return {
                projectFile: FileInfo.joinPaths(sourceDir, preferredNames[j]),
                isQbsProject: true
            };
        }
    }

    // Default to first one found
    console.warn("OnlineSource: Multiple .qbs files found, using: " + qbsFiles[0]);
    return {
        projectFile: FileInfo.joinPaths(sourceDir, qbsFiles[0]),
        isQbsProject: true
    };
}

function loadCache(cacheFile) {
    if (!File.exists(cacheFile))
        return {};

    try {
        var file = new TextFile(cacheFile, TextFile.ReadOnly);
        var content = file.readAll();
        file.close();
        var data = JSON.parse(content);
        var lastModified = Date.parse(data["lastmodified"]);
        var now = new Date();
        if ((now - lastModified) > 30000) {
            console.info("OnlineSource: Cache expired (" + (now - lastModified) + "ms old)");
            return {};
        }
        console.info("OnlineSource: Cache good (" + (now - lastModified) + "ms old)");
        return data;
    } catch (e) {
        console.warn("OnlineSource: Failed to read cache file: " + e);
        return {};
    }
}

function saveCache(cacheFile, data) {
    try {
        data["lastmodified"] = new Date().toISOString();
        File.makePath(FileInfo.path(cacheFile));
        var file = new TextFile(cacheFile, TextFile.WriteOnly);
        file.write(JSON.stringify(data, null, 4));
        file.close();
    } catch (e) {
        console.warn("OnlineSource: Failed to write cache file: " + e);
    }
}

/**
 * fetch - Determine source directory and other properties
 */
function fetch(options) {
    var name = options.name;
    if (!name || name.trim() === "") {
        console.warn("OnlineSource: No module name provided");
        return null;
    }

    var cacheFile = FileInfo.joinPaths(options.buildDirectory, "online-source", "resolution-cache.json");
    var resolvedModules = loadCache(cacheFile);

    // Check resolution cache first
    if (resolvedModules[name]) {
        console.info("OnlineSource: Using cached resolution for '" + name + "'");
        return resolvedModules[name];
    }

    var uri = options.uri;
    if (!uri) {
        console.warn("OnlineSource: No URI configured for module '" + name + "'");
        return null;
    }

    console.info("OnlineSource: Setting up module '" + name + "' from " + uri);

    var uriInfo = parseUri(uri);
    if (!uriInfo.url && !uriInfo.localPath) {
        throw "OnlineSource: Invalid URI '" + uri + "' for module '" + name + "'";
    }

    var sourceDir = Environment.getEnv("QBS_"+ name.toUpperCase() +"_SOURCE");

    if (sourceDir && sourceDir.trim !== "") {
        console.info("OnlineSource: Using environment path " + sourceDir);
    }
    else if (uriInfo.type === "local") { // Handle local paths directly (no caching needed)
        sourceDir = uriInfo.localPath;
        if (!File.exists(sourceDir)) {
            throw "OnlineSource: Local path does not exist: " + sourceDir;
        }
        console.info("OnlineSource: Using local path " + sourceDir);
    }
    else {
        var cacheDir = getCacheDir(options.sourceCache, name, uriInfo);
        sourceDir = cacheDir;

        var alreadyCached = File.exists(cacheDir) && File.exists(FileInfo.joinPaths(cacheDir, ".onlinesource-cached"));
        if (!alreadyCached) {
            File.makePath(cacheDir);

            if (uriInfo.type === "git") {
                cloneGitRepo(uriInfo.url, uriInfo.tag, cacheDir);
            } else if (uriInfo.type === "archive") {
                downloadArchive(uriInfo.url, cacheDir);

                // Find the extracted directory (archives often have a single root folder)
                var entries = File.directoryEntries(cacheDir, File.Dirs | File.NoDotAndDotDot);
                if (entries.length === 1) {
                    sourceDir = FileInfo.joinPaths(cacheDir, entries[0]);
                }
            }

            var markerFile = new TextFile(FileInfo.joinPaths(cacheDir, ".onlinesource-cached"), TextFile.WriteOnly);
            markerFile.writeLine(new Date().toISOString());
            markerFile.writeLine(uri);
            markerFile.close();
        } else {
            console.info("OnlineSource: Using cached version at " + cacheDir);
        }
    }

    var qbsInfo = detectQbsProject(sourceDir, options.projectFile);
    if (qbsInfo) {
        console.info("OnlineSource: Detected Qbs project: " + qbsInfo.projectFile);
    }

    var result = {
        sourceDirectory: sourceDir,
        projectFile: (qbsInfo && qbsInfo.isQbsProject) ? qbsInfo.projectFile : null
    };

    resolvedModules[name] = result;
    saveCache(cacheFile, resolvedModules);
    return result;
}
