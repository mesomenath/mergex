This command line tool allow you to merge XML files with the same root node. It either merge the content in one root node, or encapsulate all source root nodes in a new parent root node.
```
mergex v2.1:
        Merge XML files to std output or an output file.
        ALL XML should have the same xml prolog and root node.

Syntax:
        mergex PARAMS <XML_FILE> <XML_FILE> ...

Parameters:

        -o<OUTPUT_XML_FILE>     Path of the XML output file.

        -p[ParentRootName]      Add a parent root. If ParentRootName is not provided,
                                the name of a root postfixed with an 's' is used.

        -a[ParentRootAttribute] Add an attribute to the parent root.
                                ParentRootAttribute should be a valid xml attribute
                                defined with single quote.
                                It will only work with the -p parameter.

Samples:

        mergex *.xml
        mergex a.xml b.xml c.xml -ooutput.xml
        mergex *.xml -p -ooutput.xml
        mergex *.xml -pROOT
        mergex -ooutput.xml -pROOT a.xml b.xml c.xml
        mergex -ooutput.xml -pROOT -avalue='test' *.xml
        mergex -ooutput.xml -p -avalue='test' -aAnotherAttribute='anothertest' *.xml

```

