%{
Robot controller script

Author: Marco Medrano
Date: 27/03/22
%}

clear; close all; clc;

%% Send messages to esp32 robot controller over http requests
%{
import matlab.net.*
import matlab.net.http.*
r = RequestMessage;
uri = URI('http://192.168.100.9/?joint=1&value=120&');
resp = send(r,uri);
%}

%% Create rigid body model for the robot
dhparams = [0.619 0 7.34 0;
            0 0 1.783 -pi/2;
            0 -pi/2 0 0;
            0 pi/2 10.2 0;
            0 -pi/2 0 0;
            0 pi/2 12.9 0;
            0 -pi/2 0 0;
            0 pi/2 5.84 pi/2;
            0 0 11.0 0];
robot = rigidBodyTree;
body1 = rigidBody('body1');
jnt1 = rigidBodyJoint('jnt1','revolute');

setFixedTransform(jnt1,dhparams(1,:),'dh');
body1.Joint = jnt1;

addBody(robot,body1,'base')
body2 = rigidBody('body2');
jnt2 = rigidBodyJoint('jnt2','revolute');
body3 = rigidBody('body3');
jnt3 = rigidBodyJoint('jnt3','revolute');
body4 = rigidBody('body4');
jnt4 = rigidBodyJoint('jnt4','revolute');
body5 = rigidBody('body5');
jnt5 = rigidBodyJoint('jnt5','revolute');
body6 = rigidBody('body6');
jnt6 = rigidBodyJoint('jnt6','revolute');
body7 = rigidBody('body7');
jnt7 = rigidBodyJoint('jnt7','revolute');
body8 = rigidBody('body8');
jnt8 = rigidBodyJoint('jnt8','revolute');
body9 = rigidBody('body9');
jnt9 = rigidBodyJoint('jnt9','revolute');


setFixedTransform(jnt2,dhparams(2,:),'dh');
setFixedTransform(jnt3,dhparams(3,:),'dh');
setFixedTransform(jnt4,dhparams(4,:),'dh');
setFixedTransform(jnt5,dhparams(5,:),'dh');
setFixedTransform(jnt6,dhparams(6,:),'dh');
setFixedTransform(jnt7,dhparams(7,:),'dh');
setFixedTransform(jnt8,dhparams(8,:),'dh');
setFixedTransform(jnt9,dhparams(9,:),'dh');

body2.Joint = jnt2;
body3.Joint = jnt3;
body4.Joint = jnt4;
body5.Joint = jnt5;
body6.Joint = jnt6;
body7.Joint = jnt7;
body8.Joint = jnt8;
body9.Joint = jnt9;

addBody(robot,body2,'body1')
addBody(robot,body3,'body2')
addBody(robot,body4,'body3')
addBody(robot,body5,'body4')
addBody(robot,body6,'body5')
addBody(robot,body7,'body6')
addBody(robot,body8,'body7')
addBody(robot,body9,'body8')
showdetails(robot)
%show(robot);
axis([-15,15,-15,15,-15,15])
figure("Name","Interactive GUI")
gui = interactiveRigidBodyTree(robot,"MarkerScaleFactor",0.25);
