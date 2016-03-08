/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ui.h - An application to let users comminucate in a chat room
--
-- PROGRAM: Chat
--
-- FUNCTIONS:
-- int main(int argc, char *argv[])
-- void loadStyleSheet(QApplication* a)
--
-- DATE: March 6, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer
--
-- NOTES:
-- The file holds the main entry point of the program, creates the UI for the program styles it and displays it.
----------------------------------------------------------------------------------------------------------------------*/
#include "ui.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

void loadStyleSheet(QApplication* application);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 6, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer
--
-- INTERFACE:    int main(int argc, char *argv[])
--                      argc: number of command line arguments
--                      *argv[]: array of command line arguments
--
-- RETURNS: Value of application execution
--
-- NOTES:
-- Main entry point of the program
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadStyleSheet(&a);
    UI w;
    w.show();

    return a.exec();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateChatMenu
--
-- DATE: March 6, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer
--
-- INTERFACE:    void loadStyleSheet(QApplication* application)
--                          application: Pointer to the applicaiton to load the stylesheet into.
--
--
-- RETURNS: void.
--
-- NOTES:
-- Loads in a black stylesheet.
----------------------------------------------------------------------------------------------------------------------*/
void loadStyleSheet(QApplication* application) {
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        application->setStyleSheet(ts.readAll());
    }
}
