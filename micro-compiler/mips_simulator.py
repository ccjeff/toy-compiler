import sys
import re

class Simulator():
    def __init__(self, filename):
        self.filename = filename
        self.operations = []
        self.memory = [0 for i in range(1000)]
        self.registerIDs = {
            "$zero": 0,
            "$at" : 1,
            "$v0" : 2,
            "$v1" : 3,
            "$a0" : 4,
            "$a1" : 5,
            "$a2" : 6,
            "$a3" : 7,
            "$t0" : 8,
            "$t1" : 9,
            "$t2" : 10,
            "$t3" : 11,
            "$t4" : 12,
            "$t5" : 13,
            "$t6" : 14,
            "$t7" : 15,
            "$s0" : 16,
            "$s1" : 17,
            "$s2" : 18,
            "$s3" : 19,
            "$s4" : 20,
            "$s5" : 21,
            "$s6" : 22,
            "$s7" : 23,
            "$t8" : 24,
            "$t9" : 25,
            "$k0" : 26,
            "$k1" : 27,
            "$gp" : 28,
            "$sp" : 29,
            "$fp" : 30,
            "$ra" : 31
        }
        self.registerVals = [0 for i in range(32)]
    
    def readFile(self):
        with open(self.filename) as f:
            for line in f:
                self.operations.append(line.rstrip())
        
    def simulate(self, line):
        line_split = line.split(" ")
        opcode = line_split[0]
        if (opcode == "syscall"):
            instVal = self.registerVals[2]
            if (instVal == 1):
                #print int
                toPrint = self.registerVals[4]
                print(toPrint)
            elif (instVal == 5):
                toStore = input()
                if (not toStore.isdigit()):
                    print("ONLY SUPPORT INT INPUT")
                    toStore = input()
                self.registerVals[2] = toStore
            else:
                sys.exit("Runtime Error at syscall")
            regList = []
        else: 
            regList = line_split[1].split(",")
        ## some of the lines do not follow the 2 reg pattern, e.g. the 4($fp) in addr
        ## switch on regList length
        if (len(regList) == 2):
            ## 2 regs type, need another check
            if (opcode == "move"):
                srcIdx = self.registerIDs.get(regList[1])
                srcVal = self.registerVals[srcIdx]
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = srcVal
            elif (opcode == "sw"):
                destAddr = re.findall("[1-9][0-9]*", regList[1])[0]
                offset = int(int(destAddr)/4 - 1)
                srcIdx = self.registerIDs.get(regList[0])
                srcVal = self.registerVals[srcIdx]
                self.memory[offset] = srcVal
            elif (opcode == "lw"):
                srcAddr = re.findall("[1-9][0-9]*", regList[1])[0]
                offset = int(int(srcAddr)/4 - 1)
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = self.memory[offset]
            elif (opcode == "li"):
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = int(regList[1])
            else: 
                print("Unknown OPCODE")
                sys.exit("Runtime Error")
        elif (len(regList) == 3):
            #dealing with add/addi
            if (opcode == "add"):
                srcIdx1 = self.registerIDs.get(regList[1])
                srcIdx2 = self.registerIDs.get(regList[2])
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = int(self.registerVals[srcIdx1]) + int(self.registerVals[srcIdx2])
            elif (opcode == "addi"):
                srcIdx1 = self.registerIDs.get(regList[1])
                srcImm = int(regList[2])
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = self.registerVals[srcIdx1] + srcImm
            elif (opcode == "sub"):
                srcIdx1 = self.registerIDs.get(regList[1])
                srcIdx2 = self.registerIDs.get(regList[2])
                destIdx = self.registerIDs.get(regList[0])
                self.registerVals[destIdx] = int(self.registerVals[srcIdx1]) - int(self.registerVals[srcIdx2])
            else:
                print("Unknown OPCODE")
                sys.exit("Runtime Error")
    
    def simulateRoutine(self):
        self.readFile()
        for i in self.operations:
            self.simulate(i)

if __name__ == "__main__":
    MIPSSimulator = Simulator("output.asm")
    MIPSSimulator.simulateRoutine()
