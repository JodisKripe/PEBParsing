# PEBParsing
PEBParsing parses the environment block of the running process. No Win32 API is being called.

This was a exploring project into including assembly into a c program. Inspiration and concepts taken from [Boku7's whereami](https://github.com/boku7/whereami) BOF.

Current the various files here perform the following functions:
- [x] Print Environmental Variables
- [x] Print Process Information
- [x] Fetches debugging Information
- [x] Prints loaded modules

Will convert these into a BOF too hopefully.

All compilation is using gcc