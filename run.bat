:run.bat
:


start Server/Debug/Executive.exe localhost 2001 Server\TESTFOLDER2
start Server/Debug/Executive.exe localhost 2002 Server\TESTFOLDER2
start Client1/Debug/Executive.exe localhost 1001 localhost 2001 Client1/TESTFOLDER1 textfile.txt
start Client2/Debug/Executive.exe localhost 1002 localhost 2001 Client2/TESTFOLDER1 wordfile.docx