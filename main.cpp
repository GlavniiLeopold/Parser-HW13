#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include "parser.h"

class FileManager
{
private:
    QString path;
    Parser *parser;
public:
    FileManager(){parser = new Parser();}
    void get_arguments(QStringList args)
    {
        if(args.size() < 2)
        {
            path = QCoreApplication::applicationDirPath() + "/sentence.txt";
            std::cerr << "error - 1 command line arguments needed\n";
            std::cerr << "path set to " << path.toStdString() << "\n";
        }
        else
            path = args.last();
    }
    bool parse_contents()
    {
        if(path.isEmpty())
            return 0;
        QFile inputFile(path);
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QString data;
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                data.append(in.readLine()+" ");
            }
            parser->parse(data);
            parser->print_tree_nodes();
            inputFile.close();
            return 1;
        }
        std::cerr << "can't open the file at" << path.toStdString() << "\n";
        return 0;
    }
    void read_from_console()
    {
        QTextStream stream(stdin);
        while(1)
        {
            std::cout << "\nEnter you sentence \n";
            parser->parse(stream.readLine());
            parser->print_tree_nodes();
        }
    }

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FileManager *manager = new FileManager();
    //вывод через дерево tree внутри parser, реализованное через вектор

    QStringList command_line = a.arguments();
    manager->get_arguments(command_line);
    if(!manager->parse_contents())
        manager->read_from_console();
    return a.exec();
}
