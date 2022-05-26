// myDisassembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>

std::string whichRegister(int regInputValue);

int main(int argc, char* argv[])
{
    //FOR .EXE:
    //std::string inputFileName = argv[1]; //input argument to variable inputFileName

    //MANUAL ENTRY, REPLACE *.OBJ WITH FILE NAME:
    std::string inputFileName = "test_case4.obj"; //input argument to variable inputFileName

    std::ifstream inFile(inputFileName); //file with name inputFileName

    if (inFile.is_open()) //if File Can Read
    {
        //Create output file with same name and .s extension
        std::string outputFileName = inputFileName;
        outputFileName.resize(outputFileName.size() - 3);
        outputFileName.push_back('s'); //file with name outputFileName
        //std::ofstream outFile(outputFileName);

        int ERROR_FLAG = 0;

        //Line INFO
        std::string line;                   //Lines of the input file
        std::string line_bin;                   //Lines of the input file (in Binary)
        int currentLine = 0;             //current line of file being read (zero indexed)
        int totalLineNum = 0;            //number of lines in input file
        std::stringstream ss;
        std::string output_line;
        std::vector<std::string> output_vector;

        //to get hex from decimal:
        //ss << std::hex << 10; std::cout << ss.str();

        //Branch INFO
        int totalBranches = 0;
        int currentBranch = 0;
        std::vector<std::string> BranchAddrLabels;
        std::vector<int> BranchAddrLine;
        std::string AddrName;

        unsigned int data;                  //for 32 bit values from input file
        
        //general
        std::string OpCode;
        std::string rS_String;
        int rS;
        std::string rT_String;
        int rT;


        //r type
        std::string rD_String;
        int rD;
        std::string shamt_String;
        int shamt;
        std::string funct;

        //i type
        std::string imm_String;
        int imm;
        signed short Offset;

        //Check for branches
        std::cout << "SEARCHING FOR BRANCHES\n-----------------------\n";
        while (std::getline(inFile, line))
        {

            data = stoul(line, nullptr, 16);            //Hex value to decimal
            std::cout << "Line " << currentLine+1 << " code: "<< line << std::endl;
            line = std::bitset<32>(data).to_string();   //decimal value to binary string
            OpCode = line.substr(0,6);                  //the OpCode

            //Branch Equal found
            if ((OpCode == "000100") || (OpCode == "000101"))
            {
                std::cout << std::endl;

                //report found
                std::cout << "\tBEQ found on line " << currentLine+1 << std::endl;

                //grab offset
                imm_String = line.substr(16, 16);
                Offset = (signed short)(stoul(imm_String, nullptr, 2));
                std::cout << "\tImm = " << imm_String << "\n\tIn signed Decimal: " << Offset << std::endl;

                totalBranches++;

                ss << std::hex << ((currentLine + 1 + Offset) * 4);
                std::string temp_AddrStr = ss.str();
                while (temp_AddrStr.size() < 4)
                {
                    temp_AddrStr = "0" + temp_AddrStr;
                AddrName = "Addr_" + temp_AddrStr;
                }
                //AddrName = "Addr_" + ss.str();
                ss.str("");
                std::cout << "\tLine/Label: " << (currentLine + 1 + Offset) << "/" << AddrName << std::endl;
                BranchAddrLabels.push_back(AddrName);
                BranchAddrLine.push_back(currentLine + 1 + Offset);

                std::cout << std::endl;
            }

            currentLine++;
        }
        currentLine = 0;

        inFile.close();
        inFile.open(inputFileName);
        std::cout << "\nPARSING CODES + SEARCHING FOR ERRORS\n-----------------------\n";
        while (std::getline(inFile, line))
        {
            int i = 0;
            int whichBranchLabel = -1;
            while (i < BranchAddrLine.size())
            {
                if (BranchAddrLine[i] == currentLine)
                {
                    output_vector.push_back(BranchAddrLabels[i]);
                    //outFile << BranchAddrLabels[i] << ":" << std::endl;
                    i = BranchAddrLine.size();
                }
                else
                {
                    i++;
                }
            }

            i = 0;
            whichBranchLabel = -1;
            currentLine++;

            if (line.size() == 8)
            {
                data = stoul(line, nullptr, 16);            //Hex value to decimal
                std::cout << "Line " << currentLine << " code: " << line << std::endl;
                line_bin = std::bitset<32>(data).to_string();   //decimal value to binary string

                output_line = "";

                //Retrieve the OpCode:
                OpCode = line_bin.substr(0, 6);
                rS_String = line_bin.substr(6, 5);
                rS = stoi(rS_String, nullptr, 2);
                rT_String = line_bin.substr(11, 5);
                rT = stoi(rT_String, nullptr, 2);

                //r type
                rD_String = line_bin.substr(16, 5);
                rD = stoi(rD_String, nullptr, 2);
                shamt_String = line_bin.substr(21, 5);
                shamt = stoi(shamt_String, nullptr, 2);

                funct = line_bin.substr(26, 6);

                //i type
                imm_String = line_bin.substr(16, 16);
                imm = (int)((signed short)(stoul(imm_String, nullptr, 2)));

                // ADD INSTRUCTION add
                if ((OpCode == "000000") && (funct == "100000"))
                {
                    std::cout << "\tadd found\n\n";

                    output_line = "\tadd\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // ADD IMMEDIATE INSTRUCTION addi
                else if ((OpCode == "001000"))
                {
                    std::cout << "\taddi found\n\n";

                    output_line = "\taddi\t" + whichRegister(rT) + ", " + whichRegister(rS) + ", " + std::to_string(imm);
                }
                // ADD IMMEDIATE UNSIGNED INSTRUCTION addiu
                else if ((OpCode == "001001"))
                {
                    std::cout << "\taddiu found\n\n";

                    output_line = "\taddiu\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                else if ((OpCode == "000000") && (funct == "100001"))
                {
                    std::cout << "\taddu found\n\n";

                    output_line = "\taddu\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // AND INSTRUCTION and
                else if ((OpCode == "000000") && (funct == "100100"))
                {
                    std::cout << "\tand found\n\n";

                    output_line = "\tand\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // AND IMMEDIATE INSTRUCTION andi
                else if ((OpCode == "001100"))
                {
                    std::cout << "\tandi found\n\n";

                    output_line = "\tandi\t" + whichRegister(rT) + ", " + whichRegister(rS) + ", " + std::to_string(imm);
                }
                // BRANCH ON EQUAL INSTRUCTION beq
                else if ((OpCode == "000100"))
                {
                    std::cout << "\tbeq found\n\n";

                    output_line = "\tbeq\t" + whichRegister(rS) + ", " + whichRegister(rT) + ", " + BranchAddrLabels[currentBranch];
                    currentBranch++;
                }
                // BRANCH ON NOT EQUAL INSTRUCTION bne
                else if ((OpCode == "000101"))
                {
                    std::cout << "\tbne found\n\n";

                    output_line = "\tbne\t" + whichRegister(rS) + ", " + whichRegister(rT) + ", " +  BranchAddrLabels[currentBranch];
                    currentBranch++;
                }
                // LOAD BYTE UNSIGNED INSTRUCTION lbu
                else if ((OpCode == "100100"))
                {
                    std::cout << "\tlbu found\n\n";

                    output_line = "\tlbu\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // LOAD HALFWORD UNSIGNED INSTRUCTION lhu
                else if ((OpCode == "100101"))
                {
                    std::cout << "\tlhu found\n\n";

                    output_line = "\tlbu\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // LOAD LINKED INSTRUCTION ll
                else if ((OpCode == "110000"))
                {
                    std::cout << "\tll found\n\n";

                    output_line = "\tll\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // LOAD UPPER IMM INSTRUCTION lui
                else if ((OpCode == "001111"))
                {
                    std::cout << "\tlui found\n\n";

                    output_line = "\tlbu\t" + whichRegister(rT) + ", " + std::to_string(imm);
                }
                // LOAD WORD INSTRUCTION lw 
                else if ((OpCode == "100011"))
                {
                    std::cout << "\tlw found\n\n";

                    output_line = "\tlw\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // NOR INSTRUCTION nor
                else if ((OpCode == "000000") && (funct == "100111"))
                {
                    std::cout << "\tnor found\n\n";

                    output_line = "\tnor\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // OR INSTRUCTION or
                else if ((OpCode == "000000") && (funct == "100101"))
                {
                    std::cout << "\tor found\n\n";

                    output_line = "\tor\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // OR IMMEDIATE INSTRUCTION ori
                else if ((OpCode == "001101"))
                {
                    std::cout << "\tori found\n\n";

                    output_line = "\tori\t" + whichRegister(rT) + ", " + whichRegister(rS) + ", " + std::to_string(imm);
                }
                // SET LESS THAN INSTRUCTION slt
                else if ((OpCode == "000000") && (funct == "101010"))
                {
                    std::cout << "\tslt found\n\n";

                    output_line = "\tslt\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // SET LESS THAN IMM INSTRUCTION slti
                else if ((OpCode == "001010"))
                {
                    std::cout << "\tslti found\n\n";

                    output_line = "\tslti\t" + whichRegister(rT) + ", " + whichRegister(rS) + ", " + std::to_string(imm);
                }
                // SET LESS THAN IMM UNSIGNED INSTRUCTION sltiu
                else if ((OpCode == "001011"))
                {
                    std::cout << "\tsltiu found\n\n";

                    output_line = "\tsltiu\t" + whichRegister(rT) + ", " + whichRegister(rS) + ", " + std::to_string(imm);
                }
                // SET LESS THAN UNSIGNED INSTRUCTION sltu
                else if ((OpCode == "000000") && (funct == "101011"))
                {
                    std::cout << "\tsltu found\n\n";

                    output_line = "\tsltu\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // SHIFT LEFT LOGICAL INSTRUCTION sll
                else if ((OpCode == "000000") && (funct == "000000"))
                {
                    std::cout << "\tsll found\n\n";

                    output_line = "\tsll\t" + whichRegister(rD) + ", " + whichRegister(rT) + ", " + std::to_string(shamt);
                }
                // SHIFT RIGHT LOGICAL INSTRUCTION srl
                else if ((OpCode == "000000") && (funct == "000010"))
                {
                    std::cout << "\tsrl found\n\n";

                    output_line = "\tsrl\t" + whichRegister(rD) + ", " + whichRegister(rT) + ", " + std::to_string(shamt);
                }
                // STORE BYTE INSTRUCTION sb
                else if ((OpCode == "101000"))
                {
                    std::cout << "\tsb found\n\n";

                    output_line = "\tsb\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // STORE CONDITIONAL INSTRUCTION sc
                else if ((OpCode == "111000"))
                {
                    std::cout << "\tsc found\n\n";

                    output_line = "\tsc\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // STORE HALFWORD INSTRUCTION sh
                else if ((OpCode == "101001"))
                {
                    std::cout << "\tsh found\n\n";

                    output_line = "\tsh\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // STORE WORD INSTRUCTION sw
                else if ((OpCode == "101011"))
                {
                    std::cout << "\tsw found\n\n";

                    output_line = "\tsw\t" + whichRegister(rT) + ", " + std::to_string(imm) + "(" + whichRegister(rS) + ")";
                }
                // SUBTRACT INSTRUCTION sub
                else if ((OpCode == "000000") && (funct == "100010"))
                {
                    std::cout << "\tsub found\n\n";

                    output_line = "\tsub\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                // SUBTRACT UNSIGNED INSTRUCTION subu
                else if ((OpCode == "000000") && (funct == "100011"))
                {
                    std::cout << "\tsubu found\n\n";

                    output_line = "\tsubu\t" + whichRegister(rD) + ", " + whichRegister(rS) + ", " + whichRegister(rT);
                }
                //INVALID FUNCTION CODE
                else if ((OpCode == "000000"))
                {
                    std::cout << "\tERROR: Cannot dissasemble " << line << " at line " << currentLine << std::endl << std::endl;

                    output_line = "\tINVALID FUNCTION CODE: " + funct;

                    ERROR_FLAG = -1;
                }
                //INVALID OP CODE
                else
                {
                    std::cout << "\tERROR: Cannot dissasemble " << line << " at line " << currentLine << std::endl << std::endl;

                    output_line = "\tINVALID OPCODE: " + OpCode;

                    ERROR_FLAG = -1;
                }

                if (output_line != "")
                {
                    output_vector.push_back(output_line);
                    //outFile << output_line << std::endl;
                }
            }
            else
            {
                std::cout << "\tERROR: Cannot dissasemble " << line << " at line " << currentLine << std::endl << std::endl;
                output_vector.push_back("\tINVALID INSTRUCTION SIZE");
                
                //outFile << "\tINVALID INSTRUCTION SIZE" << std::endl;
                ERROR_FLAG = -1;
            }
        }
        if (ERROR_FLAG == 0)
        {
            std::ofstream outFile(outputFileName);

            for (int j = 0; j < output_vector.size(); j++) // FINISH
            {
                outFile << output_vector[j] << std::endl;
            }

            std::cout << "Successfully created " << outputFileName << std::endl;
        }
        else
        {
            std::cout << "ERRORS FOUND WHILE DISASSEMBLING\n    -No " << outputFileName << " file created." << std::endl;
        }
        std::cout << "\n...ending disassembly process" << std::endl;

        //BRANCH LABELS PRINT FOR DEBUG


        /*for (std::vector<std::string>::const_iterator i = BranchAddrLabels.begin(); i != BranchAddrLabels.end(); ++i)
            std::cout << *i << ' ';*/
    }
    else
    {
        std::cout << "INVALID FILE NAME\n";
    }

}

//FUNCTIONS
std::string whichRegister(int regInputValue) //register value
{
    std::string registerOut;
    if (regInputValue == 0)
        registerOut = "$zero";
    else if (regInputValue == 1)
        registerOut = "$at";
    else if (regInputValue == 2)
        registerOut = "$v0";
    else if (regInputValue == 3)
        registerOut = "$v1";
    else if (regInputValue == 4)
        registerOut = "$a0";
    else if (regInputValue == 5)
        registerOut = "$a1";
    else if (regInputValue == 6)
        registerOut = "$a2";
    else if (regInputValue == 7)
        registerOut = "$a3";
    else if (regInputValue == 8)
        registerOut = "$t0";
    else if (regInputValue == 9)
        registerOut = "$t1";
    else if (regInputValue == 10)
        registerOut = "$t2";
    else if (regInputValue == 11)
        registerOut = "$t3";
    else if (regInputValue == 12)
        registerOut = "$t4";
    else if (regInputValue == 13)
        registerOut = "$t5";
    else if (regInputValue == 14)
        registerOut = "$t6";
    else if (regInputValue == 15)
        registerOut = "$t7";
    else if (regInputValue == 16)
        registerOut = "$s0";
    else if (regInputValue == 17)
        registerOut = "$s1";
    else if (regInputValue == 18)
        registerOut = "$s2";
    else if (regInputValue == 19)
        registerOut = "$s3";
    else if (regInputValue == 20)
        registerOut = "$s4";
    else if (regInputValue == 21)
        registerOut = "$s5";
    else if (regInputValue == 22)
        registerOut = "$s6";
    else if (regInputValue == 23)
        registerOut = "$s7";
    else if (regInputValue == 24)
        registerOut = "$t8";
    else if (regInputValue == 25)
        registerOut = "$t9";
    else if (regInputValue == 26)
        registerOut = "$k0";
    else if (regInputValue == 27)
        registerOut = "$k1";
    else if (regInputValue == 28)
        registerOut = "$gp";
    else if (regInputValue == 29)
        registerOut = "$sp";
    else if (regInputValue == 30)
        registerOut = "$fp";
    else
        registerOut = "$ra";

    return registerOut;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
