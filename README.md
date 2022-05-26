# DissassemblerCode
A MIPS core instruction disassembler using C++

ECE 2500 (Computer Organization and Architecture) Project 1

Running code (.exe, or in visual studio):
1. Download the project as a .zip and extract.

2. Ensure you save any of the .obj files to be tested to the extracted folder

3. Open the Command Prompt window (search cmd in windows search bar).

4. In the Command Prompt window, navigate to the extracted folder

5. Run the myDisassembler.exe program by typing: myDisassembler *.obj
	(* is replaced with the file name)

6. Errors and progress are written to the command line, and the output *.s file is written to the same folder the .exe is in.
	(* is replaced with the file name)

7. If this does not work, you can open the program in Visual Studio:
	1. Open Visual Studio
	2. Select "Open a project or solution"
	4. Select "myDisassembler.sln"
	5. Open myDisassembler.cpp if it isn't already
	6. Ensure you save the .obj files to be tested to the myDisassembler folder where the project/solution is.
	7. Manually type in the file number where "*.obj" is.
		(On line 19, Under the comment: "//MANUAL ENTRY, REPLACE *.OBJ WITH FILE NAME:")
	8. Click: Debug > Start Debugging
		...or the Local Windows Debugger button
	9. The output .s file is written to the folder where the project/solution is.
