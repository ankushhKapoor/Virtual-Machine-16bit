
# **Virtual Machine - 16-bit CPU Simulator**  
*A minimalistic virtual CPU designed for low-level programming and experimentation.*

<h3 align="center"><samp>I have named this virtual machine kapoorvm, reflecting my personal touch and dedication to this project.</samp></h3>


## ✨ **Features**
- 🖥️ **Registers**: General-purpose (`AX`, `BX`, `CX`, `DX`) and specialized (`SP`, `IP`).
- 📦 **Memory Management**: Supports **65 KB of virtual memory** for program execution.
- 🎛️ **Custom Instruction Set**: Implements operations like data transfer, arithmetic, and more via **opcodes**.
- 🔍 **Sub-register Access**: Direct manipulation of high and low parts of registers (e.g., `AH`, `AL`).



## 🛠️ **Architecture Overview**
### **Registers**
| Register | Type            | 
|----------|-----------------|
| `AX`     | General-purpose | 
| `BX`     | General-purpose | 
| `CX`     | General-purpose | 
| `DX`     | General-purpose | 
| `SP`     | Stack pointer   | 
| `IP`     | Instruction ptr | 

### **Memory**
- **16-bit Address Space**: Total virtual memory of **65 KB**.  

### **Instruction Set**
- Custom-designed opcodes for core CPU operations like `MOV`, `HLT`, and more.  



## ⚠️ **Project Status**
This project is a **work in progress**.  
I’m actively:  
- 🚀 Adding new features  
- 🔧 Fixing bugs  
- 📈 Expanding the instruction set  

### Stay tuned for updates and enhancements!



## 📂 **Project Structure**
- **`src/`**: Source code for the VM.  
- **`include/`**: Header files for modularity.  
- **`tests/`**: Test cases for validating functionality.  


## 🖥️ **Getting Started**
### **Prerequisites**
- **OS**: Linux (recommended). 
- **C Compiler**: GCC or Clang.

### **Build Instructions**
1. Clone the repository:
   ```bash
   git clone https://github.com/Ankush1626/Virtual-Machine-16bit.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Virtual-Machine-16bit
   ```
3. Compile the source code:
   ```bash
   gcc -o vm src/main.c
   ```
4. Run the virtual machine:
   ```bash
   ./vm
   ```



## 📚 **Usage**
- Load a program into memory with simple assembly-like instructions.  
- Use the custom instruction set to execute operations and observe the results.



## 🌟 **Planned Features**
- 📜 **Stack Operations**: Add `PUSH` and `POP` instructions.  
- ➗ **Advanced Arithmetic**: Include `MUL` and `DIV` operations.  



## 🙏 **Acknowledgments**
Inspired by low-level CPU architectures and the x86 assembly language.  



### **Note**
This project is **under development** and may have missing features or bugs.  
I am continuously improving it—your feedback and support are invaluable!  

---

This project is based on the tutorial video created by **Dr. Jonas Birch** on YouTube.  
Dr. Birch is a phenomenal programmer whose work forms the foundation of this project.  
- Several header files, such as `birchutils.h`, were authored by him.  
- Definitions and `typedef`s used for typecasting in this project are directly derived from his work.  

A huge thanks to Dr. Birch for his invaluable resources and guidance!


