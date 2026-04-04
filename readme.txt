IDE used: JetBrains CLion

Version Control: Git / GitHub

GitHub Repository: https://github.com/HamadAlnassar/GridQualityReport

************ How to Run (Using CLion IDE) *************

1) Clone the Repository: Open CLion and go to File > New > Project from Version Control. Paste the GitHub link above.

2) Build the Project: Click the 'Hammer' icon or go to Build > Build Project.

3) Run the Application: Click the green 'Play' button.

4) Input: When prompted in the terminal, type the location of your data file 

5) Output: The program will process the data and automatically generate a results.txt file inside your cmake-build-debug folder.


************ How to Run (Using Command Line / Terminal) ***********

Step 1: Clone the Project
Navigate to your desired folder and run: git clone https://github.com/HamadAlnassar/GridQualityReport

Step 2: Compile the Code
Since the project uses multiple .c files and header (.h) files, you must compile all of them together. Use the following GCC command:
gcc main.c waveform.c io.c -o GridAnalyzer -lm

Step 3: Run the Program
./GridAnalyzer