
# CSOPESY-S11 Group 10 MCO1

Setup:

- Open Visual Studio Code, make sure to use C++ 20 in order for the project to work.
- Under mainProgram.cpp, uncomment the required include files (Or run the project in Visual Studio).
- Edit the config file for testing.
- Finally, run mainProgram.cpp and type 'initialize'.

How to run:

- Once mainProgram.cpp is running, type 'initialize' to initialize program arguments from config file.
- You may create a separate screen by using 'screen -s <screen name>'. This will create a new process that could be viewed in screen -ls from the main screen.
- In a generated screen, you may check the progress of the process by typing 'process -smi'
- You may also test the scheduler by using 'scheduler -test' from the main screen. This will continue to make processes with instructions within the range of min-ins and max-ins.
- You may stop the creation of processes by using 'scheduler -stop'
- To check the progress of the scheduler, you may check it by typing 'screen -ls'
- After testing, you may exit both the program and a generated screen by typing 'exit'

Notes:
- a delay was implemented in the creation of dummy processes for readability purposes
