import "onlinesource.js" as OnlineSource

Probe {
    id: probe

    property string name
    property string uri
    property string projectFile
    property path sourceCache
    property path buildDirectory: project.buildDirectory

    // Results
    property path sourceDirectory
    property path projectFilePath

    configure: {
        var result = OnlineSource.fetch({
            name: probe.name,
            uri: probe.uri,
            projectFile: probe.projectFile,
            sourceCache: probe.sourceCache,
            buildDirectory: probe.buildDirectory,
        });
        if (result) {
            sourceDirectory = result.sourceDirectory;
            if (result.projectFile) projectFilePath = result.projectFile;
            found = true;
        } else {
            found = false;
        }
    }
}
