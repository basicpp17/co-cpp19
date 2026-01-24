SubProject {
    property variant source

    condition: source && source.found
    filePath: source.projectFilePath
}
