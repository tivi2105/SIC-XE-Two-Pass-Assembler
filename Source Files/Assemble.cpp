#include "PassI.h"
#include "IOHandler.h"
#include "PassII.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

ifstream ipfile;
ofstream opfile;

bool started;
bool ended;

map<string, OpInfo*>* mainOpTab;

void openInitialStreams(string in, string out) {
    ipfile.open(in.c_str());
    deleteFile(out.c_str());
    opfile.open(out.c_str(), std::ios::app);
}

void finish() {
    opfile.close();
    ipfile.close();
    //deleteOpTab();
}

bool isCSect(transformLine pl) {
    return (toUpperCase(pl.opcode).compare("CSECT") == 0) ? true : false;
}

transformLine readAndParse() {
    string before = readLine(&ipfile);
    string x = trim(before);
    return parse(x);
}

int assembleFile(Ui::MainWindow *ui, string ipPath, string opPath) {
    ui->status_tb->setText("Ready");
    ui->status_tb->setText("Assembler Started");
    openInitialStreams(ipPath,opPath);
    bool successfullPass1 = true;
    createTitle(&opfile);
    mainOpTab = getOpTab();
    vector<transformLine> currCSect;
    vector<metaData> cSects;
    transformLine pl;

    started = false;
    ended = false;

    while (!ipfile.eof()) {
        pl = readAndParse();

        if (!isCSect(pl)) {
            currCSect.push_back(pl);
            continue;
        }

        if (isCSect(pl)) {
            metaData old = runPass1(currCSect, &started, &ended,
                    &successfullPass1);
            cSects.push_back(old);
            currCSect.clear(); 
            currCSect.push_back(pl);
        }

    }

    cSects.push_back(runPass1(currCSect, &started, &ended, &successfullPass1));
    currCSect.clear();

    string opst;

    for (unsigned int i = 0; i < cSects.size(); i++) {
        for (unsigned int j = 0; j < cSects[i].dataSect.size(); j++) {
            tocreate x = cSects[i].dataSect.at(j);
            opst += "\n" +x.data;
            if (x.type == -1) {
                createError(&opfile, x.data);
                ui->passI_tb->setText(opst.c_str());
            } else if (x.type == 0) {
                createComment(&opfile, x.data);
                ui->passI_tb->setText(opst.c_str());
            } else if (x.type == 1) {
                createLine(&opfile, x.data);
                ui->passI_tb->setText(opst.c_str());
            } else {
            }
        }
        createBorder(&opfile);
        createSymTab(ui, &opfile, &cSects[i].symTab);
    }

    if (!ended) {
        createError(&opfile, "The program has no END clause");
    }

    if (!successfullPass1) {
        finish();
         ui->status_tb->setText("Errors in Input file please check listFile.txt");
        return 0;
    }

    runPass2(ui, &opfile, &cSects);

    ui->status_tb->setText("Done" );
    finish();
    return 0;

}
