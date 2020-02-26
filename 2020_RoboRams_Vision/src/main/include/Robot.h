/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <frc/TimedRobot.h>
#include "RamVision.h"


class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;
  
  static void VisionThread()
  {
      RamVision vision;
      std::cout << "I am vision\n";
      //cs::UsbCamera camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();
      //cs::CvSink cvSink = frc::CameraServer::GetInstance()->GetVideo();
      VideoCapture webcam(0);
      cs::CvSource cvOutput = frc::CameraServer::GetInstance()->PutVideo("Processed: ",320,240);

      while(true) 
      {
        if (webcam.isOpened())
        {
          webcam >> vision.currentFrame_;
          //cvSink.GrabFrame(vision.currentFrame_);
          if (vision.currentFrame_.empty() == false)
          {
            vision.UpdateFrame(vision.currentFrame_);
            if (vision.ProcessCurrentFrame(vision.x,vision.y,vision.processedFrame_))
            {
              cvOutput.PutFrame(vision.processedFrame_);
              std::cout << "Mid x: " << vision.x << std::endl;
            }
          }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(30));
      }
  }
 
};