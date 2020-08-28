#include "mainwindow.h"
#include "supporto/combotype.h"
#include <QMessageBox>
#include <QString>
#include "supporto/infolayout.h"
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include "supporto/nievaexception.h"
using std::string;

MainWindow::MainWindow(Model* m, QWidget *parent): QWidget(parent), menu(new MenuBarTrain(this)), modello(m), layout(new MainLayout(this)), layoutAdd(nullptr), layoutMod(nullptr)
{
    setWindowTitle("Nieva Trains");
    QHBoxLayout* mainLayout= new QHBoxLayout(this);
    mainLayout->addWidget(layout);
    mainLayout->setMenuBar(menu);
    setLayout(mainLayout);
}
void MainWindow::refreshList(){
    layout->getList()->clear();
    for(unsigned int i=0; i<modello->numerotreni(); i++)
        layout->getList()->addTrenoList(modello->getTreno(i));
}
void MainWindow::slotShowInfoGenerali(){
    //occhio che magari il puntatore viene cancellato all'uscita ma non l'oggetto
    QString str1= "La quantità di treni presenti nel sistema è:  "+QString::number(modello->numerotreni())+"treni";
    QString str2= "Il peso medio dei treni nel sistema è:  "+QString::number(modello->getPesoM())+"t";
    QString str3= "La velocità media dei treni nel sistema è:  "+QString::number(modello->getSpeedM())+"km/h";
    QString str4= "Il peso maggiore registrato è:  "+QString::number(modello->getPesoT())+"t";
    QString str5= "La velocità maggiore registrata è:  "+QString::number(modello->getSpeedT())+"km/h";

    InfoLayout* info=new InfoLayout(this,str1,str2,str3,str4,str5);
    info->setMargin(13);
    info->setDimensioni(300,250);
    info->show();
}
//lista eccezioni
void MainWindow::slotCarica(){
    QString file= QFileDialog::getOpenFileName(
                this,
                tr("Choose file"),
                "../NievaTrains_P2/file risorse",
                "File JSON(*.json)"
                );
    if(file!=""){
    //reset ricerca?
        modello->clear();
        //eccezione
        try{modello->load(file.toStdString());}catch(NievaException* e){QMessageBox* tmp=new QMessageBox();tmp->setText(QString::fromStdString(e->getMessage()));tmp->setWindowTitle("Warning");tmp->show();}
        if(modello->isEmpty()){
            QMessageBox::warning(this,"Attenzione!","Il file e' vuoto.");
        }
        else{
            refreshList();
        }
    }else{
        QMessageBox* tmp=new QMessageBox();tmp->setText("Seleziona un file.");tmp->setWindowTitle("Warning");tmp->show();
    }
}
//lista eccezioni
void MainWindow::slotSalva(){
    QString file= QFileDialog::getSaveFileName(
                this,
                tr("Choose file"),
                "../NievaTrains_P2/file risorse",
                "File JSON(*.json)"
                );
    if(file!=""){
    //reset ricerca?
        //eccezione
        try{modello->save(file.toStdString());}catch(NievaException* e){QMessageBox* tmp=new QMessageBox();tmp->setText(QString::fromStdString(e->getMessage()));tmp->setWindowTitle("Warning");tmp->show();}
    }else{
        QMessageBox* tmp=new QMessageBox();tmp->setText("Seleziona un file.");tmp->setWindowTitle("Warning");tmp->show();
    }
}

void MainWindow::slotAutori()
{
    QDialog* info=new QDialog(this);
    QLabel* text=new QLabel(info);
    text->setText("Gli autori:\n Alberto Crivellari, Matteo Brosolo, Francesco Bugno.");
    text->setMargin(5);
    info->resize(300,50);
    info->show();
}


void MainWindow::slotRemoveTreno()
{
    unsigned int t=layout->estraiTrenoSelezionato();
    layout->eliminaTreno(t);
    modello->erase(t);
    refreshList();
}

void MainWindow::slotShowTreno(){
    string str="";
    if(layout->estraiTrenoSelezionato()!=-1)
        str=modello->treno2string(layout->estraiTrenoSelezionato());
    layout->stampaDettagliTreno(str);
}
void MainWindow::slotFlush(){
    layout->flushList();
    modello->clear();
}

void MainWindow::slotShowInserimentoTreno(){
    int x=layout->getTrenoInserimento();
    layoutAdd=new AggiuntaLayout(this,x);
    layoutAdd->show();
}
void MainWindow::slotInserisciTreno(){
    unsigned int x=layoutAdd->getTipo();
    std::string nome=layoutAdd->getNome();
    std::string costruttore=layoutAdd->getCostruttore();
    unsigned int speed=layoutAdd->getSpeed();
    unsigned int peso=layoutAdd->getPeso();
    if(x==0){
        double efficenzaS=layoutAdd->getEfficenzaS();
        std::string carburanteS=layoutAdd->getCarburanteS();
        Steam* train=new Steam(nome, costruttore, speed, peso, efficenzaS, carburanteS);
        modello->push_end(train);
    }else if(x==1){
        double efficenzaE=layoutAdd->getEfficenzaE();
        bool trasmissione=layoutAdd->getTrasmissione();
        Electric* train=new Electric(nome, costruttore, peso, speed, trasmissione, efficenzaE);
        modello->push_end(train);
    }else if(x==2){
        std::string carburanteIC=layoutAdd->getCarburanteIC();
        double efficenzaIC=layoutAdd->getEfficenzaIC();
        Internal_Combustion* train= new Internal_Combustion(nome, costruttore, peso, speed, carburanteIC, efficenzaIC);
        modello->push_end(train);
    }else if(x==3){
        bool tecnologia=layoutAdd->getTecnologia();
        Maglev* train= new Maglev(nome, costruttore, peso, speed, tecnologia);
        modello->push_end(train);
    }else{
        double efficenzaE=layoutAdd->getEfficenzaE();
        bool trasmissione=layoutAdd->getTrasmissione();
        std::string carburanteIC=layoutAdd->getCarburanteIC();
        double efficenzaIC=layoutAdd->getEfficenzaIC();
        bool primario=layoutAdd->getPrimario();
        Bimode* train=new Bimode(nome, costruttore, peso, speed, carburanteIC, efficenzaIC, trasmissione, efficenzaE, primario);
        modello->push_end(train);
    }

    layout->getList()->addTrenoList(modello->getTreno(modello->numerotreni()-1));
    //aggiornamento lista view
    layoutAdd->hide();
    delete layoutAdd;
}
void MainWindow::slotShowModificaTreno(){
    unsigned int indecs=layout->getList()->getIndex();
    Treno* TrenoDaModificare=modello->getTreno(indecs);
    std::string tipo=TrenoDaModificare->type();

    if(tipo=="Electric"){
        layoutMod=new ModificaLayout(this,1,indecs);
        Electric*tmp=dynamic_cast<Electric*>(TrenoDaModificare);
        layoutMod->setEfficenzaE(tmp->getEfficenzaElettrico());
        layoutMod->setTrasmissione(tmp->getTrasmissioneElettrico());
    }
    else if(tipo=="Bimode"){
        layoutMod=new ModificaLayout(this,4,indecs);
        Bimode*tmp=dynamic_cast<Bimode*>(TrenoDaModificare);
        layoutMod->setPrimario(tmp->getMotorePrimario());
        layoutMod->setEfficenzaIC(tmp->getEfficenzaIC());
        layoutMod->setCarburanteIC(tmp->getCarburanteIC());
        layoutMod->setEfficenzaE(tmp->getEfficenzaElettrico());
        layoutMod->setTrasmissione(tmp->getTrasmissioneElettrico());
    }
    else if(tipo=="Steam"){
        layoutMod=new ModificaLayout(this,0,indecs);
        Steam*tmp=static_cast<Steam*>(TrenoDaModificare);
        layoutMod->setEfficenzaS(tmp->getEfficenzaSteam());
        layoutMod->setCarburanteS(tmp->getCarburanteSteam());
    }
    else if (tipo=="Internal_Combustion"){
        layoutMod=new ModificaLayout(this,2,indecs);
        Internal_Combustion*tmp=dynamic_cast<Internal_Combustion*>(TrenoDaModificare);
        layoutMod->setEfficenzaIC(tmp->getEfficenzaIC());
        layoutMod->setCarburanteIC(tmp->getCarburanteIC());
    }
    else if (tipo=="Maglev"){
        layoutMod=new ModificaLayout(this,3,indecs);
        Maglev*tmp=static_cast<Maglev*>(TrenoDaModificare);
        layoutMod->setTecnologia(tmp->getTecnologia());
    }
    else{
        //throw
    }
    layoutMod->setNome(TrenoDaModificare->getNome());
    layoutMod->setCostruttore(TrenoDaModificare->getCostruttore());
    layoutMod->setPeso(TrenoDaModificare->getPeso());
    layoutMod->setSpeed(TrenoDaModificare->getSpeed());

    layoutMod->show();
}
void MainWindow::slotModificaTreno(){
    unsigned int x=layoutMod->getInd();
    /*unsigned int tip=layoutMod->getTipo();
    if(tip==0){

    }else if(tip==1){

    }else if(tip==2){

    }else if(tip==3){

    }else if(tip==4){

    }else{
        //throw
    }
*/
    Treno* TrenoDaModificare=modello->getTreno(x);
    TrenoDaModificare->setNome(layoutMod->getNome());
    TrenoDaModificare->setCostruttore(layoutMod->getCostruttore());
    TrenoDaModificare->setSpeed(layoutMod->getSpeed());
    TrenoDaModificare->setPeso(layoutMod->getPeso());


    if(Steam*tmp=dynamic_cast<Steam*>(TrenoDaModificare)){
        tmp->setEfficenzaSteam(layoutMod->getEfficenzaS());
        tmp->setCarburanteSteam(layoutMod->getCarburanteS());
    }if(Electric*tmp=dynamic_cast<Electric*>(TrenoDaModificare)){
        tmp->setEfficenzaElettrico(layoutMod->getEfficenzaE());
        tmp->setTrasmissioneElettrico(layoutMod->getTrasmissione());
    }if(Internal_Combustion*tmp=dynamic_cast<Internal_Combustion*>(TrenoDaModificare)){
        tmp->setCarburanteIC(layoutMod->getCarburanteIC());
        tmp->setEfficenzaIC(layoutMod->getEfficenzaIC());
    }if(Maglev*tmp=dynamic_cast<Maglev*>(TrenoDaModificare)){
        tmp->setTecnologia(layoutMod->getTecnologia());
    }if(Bimode*tmp=dynamic_cast<Bimode*>(TrenoDaModificare)){
        tmp->setMotorePrimario(layoutMod->getPrimario());
    }
    //sostituzione treno

    //refresh lista
    refreshList();
    layout->getList()->setCurrentRow(x);
    layoutMod->hide();
    delete layoutMod;
}
MainWindow::~MainWindow()
{
}































