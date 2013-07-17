#include "RFWidget.h"

int initConfig(RCConfig *config){
    ifstream configFile;
    string line;

    configFile.open("rf-client.cnf", ios::in);

    if(!configFile.is_open()){
        cerr << "Can't open configuration file" << endl;
        return -1;
    }

    if(!config){
            cerr << "Can't parse configuration file" << endl;
            return -2;
        }

    while(!configFile.eof()){
        getline(configFile, line);

        if(line.empty()){
            continue;
        }
        if(line.find("rfclient.name") != string::npos){
            config->name = line.substr(line.find('=') + 1);

            if(config->name.size() > 16){
                cerr << "[WARNING] rfclient.name value is too long." << endl << "It will be truncated up to 16 characters." << endl;
                config->name.resize(16);
            }
        }

        if(line.find("rfclient.file_of_matlab") != string::npos){
            string fom = line.substr(line.find('=') + 1);

            if(fom.size() > 16){
                cerr << "[WARNING] rfclient.file_of_matlab value is too long." << endl << "It will be truncated up to 16 characters." << endl;
                fom.resize(16);
            }
            char *str = new char[ fom.length() + 1 ];
            strcpy(str,  fom.c_str());
            config->file_of_matlab = str;
        }

        if(line.find("rfclient.RULE_AMOUNT") != string::npos){
            config->RULE_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.RULE_LENGTH") != string::npos){
            config->RULE_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.BACK_AMOUNT") != string::npos){
            config->BACK_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
        }

        if(line.find("rfclient.BACK_LENGTH") != string::npos){
            config->BACK_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
        }
    }

    configFile.close();

    return 0;
}

RFWidget::RFWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(200, 120);

    QPushButton *quit = new QPushButton(tr("Quit"), this);
    //quit->setGeometry(62, 40, 75, 30);
    quit->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *go = new QPushButton(tr("&GO"));
    go->setFont(QFont("Times", 18, QFont::Bold));

    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(go, SIGNAL(clicked()), this, SLOT(runrf()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(go);
    topLayout->addStretch(1);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 0);
    gridLayout->addLayout(topLayout, 0, 1);
    gridLayout->setColumnStretch(1, 10);
    setLayout(gridLayout);

    RCConfig rcconfig;

    cout << "Initialization config file..." << endl;

    if(!initConfig(&rcconfig)){

//        cout << rcconfig.file_of_matlab << endl;
//        cout << rcconfig.name << endl;
//        cout << rcconfig.BACK_AMOUNT << endl;
//        cout << rcconfig.BACK_LENGTH << endl;
//        cout << rcconfig.RULE_AMOUNT << endl;
//        cout << rcconfig.RULE_LENGTH << endl;

        cout << "...successful" << endl;

        RfClient rfclien(rcconfig);

        cout << "Run rf-client" << endl;

//        rfclien.exec();
    }
    else
    {
        cerr << "...bad" << endl;
    }

}

int RFWidget::runrf()
{



    return 0;
}
