{
    "targets": [
        {
            "target_name": "recommend",
            "sources": [
                "recommend-node.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "./src"
            ],
            "libraries": [
                "<(module_root_dir)/src/lib/libNLPIR.so",
                "<(module_root_dir)/src/lib/libKeyExtract.so"
            ]
        }
    ]
}