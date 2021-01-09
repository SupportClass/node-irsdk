{
  "targets": [
    {
      "target_name": "IrSdkNodeBindings",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": { "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 },
      },
      "cflags": [ "-Wall", "-std=c++11" ],
      "sources": [ 
        "src/cpp/IrSdkNodeBindings.cpp", 
        "src/cpp/IrSdkCommand.cpp", 
        "src/cpp/IRSDKWrapper.cpp", 
        "src/cpp/IrSdkBindingHelpers.cpp" 
      ],
      "include_dirs" : ["<!(node -p \"require('node-addon-api').include_dir\")"],
      "default_configuration": "Release",
      "configurations": {
        "Release": { 
          "msvs_settings": {
            "VCCLCompilerTool": {
                "ExceptionHandling": 1
            }
          }
        }
      }
    }
  ]
}