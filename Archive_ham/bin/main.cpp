#include "lib/archive.h"
#include "parser/parser.h"

int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("Archiver");
    parser.Parse(argc, argv);

    archived_file archive;

    if (parser.GetFlag("create")) {
        archive.CreateArchive(parser.GetAllStringValues("filenames"), parser.GetStringValue("file"));
    } else if (parser.GetFlag("list")) {
        archive.ListOfFiles(parser.GetStringValue("file"));
    } else if (parser.GetFlag("extract")) {
        archive.ExtractFile(parser.GetStringValue("file"), parser.GetAllStringValues("filenames"));
    } else if (parser.GetFlag("append")) {
        archive.AddFile(parser.GetStringValue("file"), parser.GetStringValue("filenames"));
    } else if (parser.GetFlag("delete")) {
        archive.DeleteFile(parser.GetStringValue("file"), parser.GetStringValue("filenames"));
    } else if (parser.GetFlag("concatenate")) {
        archive.MergeArchives(parser.GetStringValue("file"), parser.GetAllStringValues("filenames")[0], parser.GetAllStringValues("filenames")[1]);
    }

    return 0;
}
