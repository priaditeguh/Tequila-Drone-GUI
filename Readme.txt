How to run GUI application in Qt: 

1. install Qt 5 for Linux https://wiki.qt.io/Install_Qt_5_on_Ubuntu
2. install redis https://www.rosehosting.com/blog/how-to-install-configure-and-use-redis-on-ubuntu-16-04/
3. Open the Qt application, then open the project file Tequila-Drone-GUI.pro
4. Change the directory path to your current project path in the code: mainwindow.cpp (MainWindow constructor, on_pushButton_savePlotLeft_clicked(), and on_pushButton_savePlotRight_clicked() functions)
	
	for example: 
		current -> QString fileName("/home/priaditeguh/GUI/Tequila-Drone-GUI/");
		change into -> QString fileName("/home/Desktop/Tequila-Drone-GUI/"); 
5. Run the progam (ctrl-R)
6. Have fun!
