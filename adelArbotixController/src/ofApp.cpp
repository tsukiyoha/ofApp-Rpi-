#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    //ofBackground(0,0,0); // black
    ofBackground(255,255,255); // white

    // window elements
    verdana14Font.load("verdana.ttf", 14, true, true);
    verdana14Font.setLineHeight(18.0f);
    verdana14Font.setLetterSpacing(1.037);


    // setup gui from ofxGui
    _gui.setup("Gui");
    _gui.setPosition(0 , 0);
    _gui.minimizeAll();

    //setup arbotix
    fArbotixPortName ="";
    fArbotixRate=0;
    fArduinoPortName ="";
    fArduinoRate=0;
    fTrackHead = false;
    fMotorsEnabled = false;

    arbotix = new arbotixController();
    loadArbotixConfiguration("arbotixConfig.xml");


    for (int i=0;i<kNbOfServos;i++)
    {
        //arbotix->attachServo(i);
    }

    printf("setup servos % i %i",fServosNames.size(),fServosIds.size());


    servo1.setController(arbotix);
    servo1.setName("servo1");
    servo1.setId(1);
    servo1.setSpeed(256); // 256

    servo2.setController(arbotix);
    servo2.setName("servo2");
    servo2.setId(2);
    servo2.setSpeed(50); //50

    servo3.setController(arbotix);
    servo3.setName("servo3");
    servo3.setId(3);
    servo3.setSpeed(85); //85

    servo4.setController(arbotix);
    servo4.setName("servo4");
    servo4.setId(4);
    servo4.setSpeed(128);

    servo5.setController(arbotix);
    servo5.setName("servo5");
    servo5.setId(5);
    servo5.setSpeed(128);

    printf("setup servos done");


    //setup Ossia
    printf("setup Ossia");
    fOssia.setup();
    //fOssia.setup("OSCQuery", "ofxOssiaDevice", 9000, 5678);
    fOssiaAngleControl1.setup(fOssia.get_root_node(), "Control 1");
    fOssiaAngleControl2.setup(fOssia.get_root_node(), "Control 2");
    fOssiaAngleControl3.setup(fOssia.get_root_node(), "Control 3");
    fOssiaAngleControl4.setup(fOssia.get_root_node(), "Control 4");
    fOssiaAngleControl5.setup(fOssia.get_root_node(), "Control 5");

    fOssiaHeadPositionControl.setup(fOssia.get_root_node(), "Head");
    fOssiaHeadPositionX.setup(fOssiaHeadPositionControl,"X",512,0,1024);
    fOssiaHeadPositionY.setup(fOssiaHeadPositionControl,"Y",384,0,768);


    float initialPosValue ;

    fInitialPosServo1 = ofMap(fServosInitialPos[0], fServosMins[0], fServosMax[0],0.,1.);
    printf("set initial pos to %f\n",initialPosValue);
    fOssiaAngleServo1.setup(fOssiaAngleControl1, servo1.getName(), fInitialPosServo1, 0., 1.);

    fInitialPosServo2 = ofMap(fServosInitialPos[1], fServosMins[1], fServosMax[1],0.,1.);
    fOssiaAngleServo2.setup(fOssiaAngleControl2, servo2.getName(), fInitialPosServo2, 0., 1.);

    fInitialPosServo3 = ofMap(fServosInitialPos[2], fServosMins[2], fServosMax[2],0.,1.);
    fOssiaAngleServo3.setup(fOssiaAngleControl3, servo3.getName(), fInitialPosServo3, 0., 1.);

    fInitialPosServo4 = ofMap(fServosInitialPos[3], fServosMins[3], fServosMax[3],0.,1.);
    fmeanHeadPositionY = fServosMins[3];
    fOssiaAngleServo4.setup(fOssiaAngleControl4, servo4.getName(), fInitialPosServo4, 0., 1.);

    fInitialPosServo5 = ofMap(fServosInitialPos[4], fServosMins[4], fServosMax[4],0.,1.);
    fmeanHeadPositionX = fServosMins[4];
    fOssiaAngleServo5.setup(fOssiaAngleControl5, servo5.getName(), fInitialPosServo5, 0., 1.);

    servo1.setOssiaParams(fOssia.get_root_node(), "servo1");
    servo2.setOssiaParams(fOssia.get_root_node(), "servo2");
    servo3.setOssiaParams(fOssia.get_root_node(), "servo3");
    servo4.setOssiaParams(fOssia.get_root_node(), "servo4");
    servo5.setOssiaParams(fOssia.get_root_node(), "servo5");


    // create a control zone named servo x with root node as parent
    servo1.setup(fServosMins[0], fServosMax[0]);
    servo2.setup(fServosMins[1], fServosMax[1]);
    servo3.setup(fServosMins[2], fServosMax[2]);
    servo4.setup(fServosMins[3], fServosMax[3]);
    servo5.setup(fServosMins[4], fServosMax[4]);


    //ofSetLogLevel(OF_LOG_VERBOSE);


    arbotix->connectController(fArbotixPortName,fArbotixRate);

    _gui.add (fOssia.get_root_node());
}


void ofApp::setupArduino(const int &version)
{
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    printf("received\n");
    printf("get infos\n");
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    printf("done\n");

    printf("Setup done\n");

}

//--------------------------------------------------------------
void ofApp::update(){

    int elapsedTime = ofGetElapsedTimeMillis();
    //ofLogNotice() << "Elapsed time : " <<  elapsedTime ;

    // check head position;
    if (fTrackHead==true)
    {
        //usleep(500000);

        //take mean of head positions
    //    int sumValX = 0;
    //    int sumValY = 0;

    //    int count =0;
    //    for (int i=0;i<500;i++)
    //    {
    //        sumValX +=fOssiaHeadPositionX;
    //        sumValY +=fOssiaHeadPositionY;

    //        count +=1;
    //    }

        //fmeanHeadPositionX = ofMap(1024-sumValX/count,0,1024,0.,1.);
        //fmeanHeadPositionY = ofMap(768-sumValY/count,0,768,0.,1.);

        //take head positions at each frame
        fmeanHeadPositionX = ofMap(1024-fOssiaHeadPositionX,0,1024,0.,1.);
        fmeanHeadPositionY = ofMap(768-fOssiaHeadPositionY,0,768,0.,1.);

//        fmeanHeadPositionX = ofMap(fOssiaHeadPositionX,0,1024,0.,1.);
//        fmeanHeadPositionY = ofMap(768-fOssiaHeadPositionY,0,768,0.,1.);


        fOssiaAngleServo4.set(fmeanHeadPositionY);
        fOssiaAngleServo5.set(fmeanHeadPositionX);
        printf("X HEAD : %f\n",fmeanHeadPositionX);
        printf("Y HEAD : %f\n",fmeanHeadPositionY);
    }


    // set or get servos angles

    if (fMotorsEnabled==false)
    {
        int posServo1 = servo1.getPos();
        int posServo2 = servo2.getPos();
        int posServo3 = servo3.getPos();
        int posServo4 = servo4.getPos();
        int posServo5 = servo5.getPos();

        float pos1 = ofMap(posServo1, fServosMins[0], fServosMax[0],0.,1.);
        float pos2 = ofMap(posServo2, fServosMins[1], fServosMax[1],0.,1.);
        float pos3 = ofMap(posServo3, fServosMins[2], fServosMax[2],0.,1.);
        float pos4 = ofMap(posServo4, fServosMins[3], fServosMax[3],0.,1.);
        float pos5 = ofMap(posServo5, fServosMins[4], fServosMax[4],0.,1.);

        fOssiaAngleServo1.set(pos1);
        fOssiaAngleServo2.set(pos2);
        fOssiaAngleServo3.set(pos3);
        fOssiaAngleServo4.set(pos4);
        fOssiaAngleServo5.set(pos5);
        int timeSleepMs = 300;
        usleep(timeSleepMs*1000);
    }



//    servo1.setAngle(fOssiaAngleServo1);
//    servo2.setAngle(fOssiaAngleServo2);
//    servo3.setAngle(fOssiaAngleServo3);
//    servo4.setAngle(fOssiaAngleServo4);
//    servo5.setAngle(fOssiaAngleServo5);

    //servo2.setAngle(fOssiaAngleServo2);

    //Rq : need to be called after arbotix->connect
    arbotix->update();

    //update servos
    if (arbotix->isInitialized() && fMotorsEnabled) {
         servo1.setAngle(fOssiaAngleServo1);
         servo2.setAngle(fOssiaAngleServo2);
         servo3.setAngle(fOssiaAngleServo3);
         servo4.setAngle(fOssiaAngleServo4);
         servo5.setAngle(fOssiaAngleServo5);
        servo1.update();
        servo2.update();
        servo3.update();
        servo4.update();
        servo5.update();
        arbotix->moveServos();
     }


    // check servos parameters
    //int tempServo3 = arbotix->getServoTemp(3);
    //printf ("temp servo 3 :%i\n",tempServo3);

    //bool ret = arbotix->waitForSysExMessage(SYSEX_DYNAMIXEL_GET_REGISTER, 2);


     // check servos temp

     if (elapsedTime>=2000)
     {
         // read a first value, otherwise temp is false in second reading (0x2B)
        arbotix->getDynamixelRegister(3,0x24,2);

        fServo2Temp = servo2.getTemp();
        fServo3Temp = servo3.getTemp();
        //printf ("Temp Servo 2 = %i °C\n",fServo2Temp);
        //printf ("Temp Servo 3 = %i °C\n",fServo3Temp);

        ofResetElapsedTimeCounter() ;
     }

    //arbotix->getDynamixelRegister(4,0x2B,2);
    //arbotix->getDynamixelRegister(5,0x2B,2);

    //usleep(100000);
    //ofResetElapsedTimeCounter() ;


}

void ofApp::standUp()
{
     fOssiaAngleServo3.set(1.0);
//     int timeSleepS = 1;
//     usleep(timeSleepS*1000000);
     fOssiaAngleServo2.set(1.0);
     fOssiaAngleServo4.set(0.8);
     fOssiaAngleServo5.set(0.5);


}

void ofApp::goToRest()
{

    fOssiaAngleServo3.set(fInitialPosServo3);
    fOssiaAngleServo2.set(fInitialPosServo2);
    fOssiaAngleServo4.set(fInitialPosServo4);
    fOssiaAngleServo5.set(fInitialPosServo5);
    fTrackHead = false;

}



//--------------------------------------------------------------
void ofApp::draw(){
 _gui.draw();

 fCircleButton.set(ofGetWindowWidth()-200, 100);
 fCircleButtonRadius = 25;

 if (fMotorsEnabled)
 {
    ofSetColor(ofColor::red);
    verdana14Font.drawString("Disable Motors", ofGetWindowWidth()-200 + fCircleButtonRadius + 10 , 100);
 }
 else
 {
    ofSetColor(ofColor::green);
    verdana14Font.drawString("Enable Motors", ofGetWindowWidth()- 200 + fCircleButtonRadius + 10 , 100);
 }

 ofCircle(fCircleButton, fCircleButtonRadius);

 // display temp servo 2
 ofSetColor(ofColor::black);
 if (fServo2Temp>=60 and fServo2Temp<=70)
 {
    ofSetColor(ofColor::orange);
 }
 else if (fServo2Temp>=70)
 {
     ofSetColor(ofColor::red);
 }
 verdana14Font.drawString("Temp Servo 2 :" + ofToString(fServo2Temp) + "°C",ofGetWindowWidth()- fCircleButtonRadius -200,150);


 // display temp servo 3

 ofSetColor(ofColor::black);
 if (fServo3Temp>=60 and fServo3Temp<=70)
 {
    ofSetColor(ofColor::orange);
 }
 else if (fServo3Temp>=70)
 {
     ofSetColor(ofColor::red);
 }
 verdana14Font.drawString("Temp Servo 3 :" + ofToString(fServo3Temp) + "°C",ofGetWindowWidth()- fCircleButtonRadius - 200,200);


 ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
 ofDrawBitmapStringHighlight(
         string() +
         "z - repose-toi\n" +
         "s - leve toi\n" +
         "a - asservissement moteurs\n" +
         "t - suivi visage\n"
         ,ofGetWindowWidth()- fCircleButtonRadius - 200, 250);
}


void ofApp::enableMotors(bool state)
{
    if (state==true)
    {
        fMotorsEnabled = true;
    }
    else
    {
        servo3.disable();
        servo2.disable();
        servo4.disable();
        servo5.disable();
        servo1.disable();
        fMotorsEnabled = false;
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key){

//    case 'a':
//        if (fMotorsEnabled ==false)
//        {
//            enableMotors(true);

//        }
//        else if (fMotorsEnabled==true)
//        {
//            enableMotors(false);
//        }
//        break;


    case 't' :
        if (fTrackHead ==false)
        {
            fTrackHead = true;
        }
        else if (fTrackHead==true)
        {
            fTrackHead = false;
        }
        break;

    case 's' :
        standUp();
        break;

    case 'z' :
        goToRest();
        break;

    case 'l' :
        //arbotix->setDynamixelRegister(4,0x19,2,1);
        break;


    default:
        break;
    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (fCircleButton.distance(ofPoint(x,y)) < fCircleButtonRadius) {
            bool motorsEnabled = fMotorsEnabled;
            enableMotors(!motorsEnabled);
        }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
    ofLogNotice() << "Exit app";
    arbotix->disconnect();
}


void ofApp::loadArbotixConfiguration(const std::string &fileName)
{
    std::string message;
    if( fXMLReader.loadFile(fileName) ){
        message = fileName + " loaded!";
        ofLogNotice() << message;
    }else{
        message = "unable to load " + fileName + " - check data/ folder";
        ofLogError() << message;
    }

    // find port name and baudrate
    std::string portName = fXMLReader.getValue("port::name", "");
    int rate = fXMLReader.getValue("port::rate", 0);

    if (portName == "" || rate == 0)
    {
        ofLogError() << "name of the port and baud rate not find in xml";
    }
    else
    {
        fArbotixPortName = portName ;
        fArbotixRate = rate;
    }

    // find servos names
    fServosList.clear();
    fServosNames.clear();
    fServosIds.clear();
    fServosMins.clear();
    fServosMax.clear();
    fServosInitialPos.clear();

    int nbServos = fXMLReader.getNumTags("servo");
    if (nbServos>0)
    {
        for(int i = 0; i < nbServos; i++)
        {
              fXMLReader.pushTag("servo", i);
              std::string servoName = fXMLReader.getValue("name","");
              int id = fXMLReader.getValue("id",0);
              int pinNb = fXMLReader.getValue("pinNb",0);
              int pos = fXMLReader.getValue("initialPos",0);
              int min = fXMLReader.getValue("min",0);
              int max = fXMLReader.getValue("max",300);

              ofLogNotice () << "Servo " << servoName << " id : " <<  id << " initial pos " << pos;

              if (servoName!="" && id!=0)
              {
                  fServosNames.push_back(servoName);
                  fServosIds.push_back(id);
                  fServosMins.push_back(min);
                  fServosMax.push_back(max);
                  printf("initial pos : %i\n",pos);
                  fServosInitialPos.push_back(pos);

              }
              fXMLReader.popTag();
         }

    }
}

void ofApp::loadArduinoConfiguration(const std::string &fileName)
{
    std::string message;
    if( fXMLReader.loadFile(fileName) ){
        message = fileName + " loaded!";
        ofLogNotice() << message;
    }else{
        message = "unable to load " + fileName + " - check data/ folder";
        ofLogError() << message;
    }

    // find port name and baudrate
    std::string portName = fXMLReader.getValue("port::name", "");
    int rate = fXMLReader.getValue("port::rate", 0);

    if (portName == "" || rate == 0)
    {
        ofLogError() << "name of the port and baud rate not find in xml";
    }
    else
    {
        fArbotixPortName = portName ;
        fArbotixRate = rate;
    }
}
