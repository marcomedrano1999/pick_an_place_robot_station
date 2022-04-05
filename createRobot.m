function robot = createRobot()
D1 = 0.734;
D2 = 0.1783;
D5 = 1.02;
D8 = 1.29;
D11 = 0.584;
D12 = 0.11;

%% Create rigid body model for the robot
dhparams = [0 0 D1 0;
            0 -pi/2 D2 0;
            0 0 0 0;
            0 pi/2 0 0;
            0 -pi/2 D5 0;
            0 0 0 0;
            0 pi/2 0 0;
            0 -pi/2 D8 0;
            0 0 0 0;
            0 pi/2 0 0;
            0 0 D11 0;
            0 0 D12 0];
robot = rigidBodyTree('Dataformat','row');
body1 = rigidBody('body1');
jnt1 = rigidBodyJoint('jnt1','revolute');
jnt1.PositionLimits=[-pi/2 pi/2];
setFixedTransform(jnt1,dhparams(1,:),'dh');
body1.Joint = jnt1;

addBody(robot,body1,'base')
body2 = rigidBody('body2');
jnt2 = rigidBodyJoint('jnt2','revolute');
jnt2.PositionLimits=[0 0];
body3 = rigidBody('body3');
jnt3 = rigidBodyJoint('jnt3','revolute');
jnt3.PositionLimits=[-pi/2 pi/2];
body4 = rigidBody('body4');
jnt4 = rigidBodyJoint('jnt4','revolute');
jnt4.PositionLimits=[0 0];
body5 = rigidBody('body5');
jnt5 = rigidBodyJoint('jnt5','revolute');
jnt5.PositionLimits=[0 0];
body6 = rigidBody('body6');
jnt6 = rigidBodyJoint('jnt6','revolute');
jnt6.PositionLimits=[-pi/2 pi/2];
body7 = rigidBody('body7');
jnt7 = rigidBodyJoint('jnt7','revolute');
jnt7.PositionLimits=[0 0];
body8 = rigidBody('body8');
jnt8 = rigidBodyJoint('jnt8','revolute');
jnt8.PositionLimits=[0 0];
body9 = rigidBody('body9');
jnt9 = rigidBodyJoint('jnt9','revolute');
jnt9.PositionLimits=[-pi/2 pi/2];
body10 = rigidBody('body10');
jnt10 = rigidBodyJoint('jnt10','revolute');
jnt10.PositionLimits=[0 0];
body11 = rigidBody('body11');
jnt11 = rigidBodyJoint('jnt11','revolute');
jnt11.PositionLimits=[-pi/2 pi/2];
body12 = rigidBody('EndEffector_Link');
jnt12 = rigidBodyJoint('EndEffector','revolute');
jnt12.PositionLimits=[0 0];

setFixedTransform(jnt2,dhparams(2,:),'dh');
setFixedTransform(jnt3,dhparams(3,:),'dh');
setFixedTransform(jnt4,dhparams(4,:),'dh');
setFixedTransform(jnt5,dhparams(5,:),'dh');
setFixedTransform(jnt6,dhparams(6,:),'dh');
setFixedTransform(jnt7,dhparams(7,:),'dh');
setFixedTransform(jnt8,dhparams(8,:),'dh');
setFixedTransform(jnt9,dhparams(9,:),'dh');
setFixedTransform(jnt10,dhparams(10,:),'dh');
setFixedTransform(jnt11,dhparams(11,:),'dh');
setFixedTransform(jnt12,dhparams(12,:),'dh');

body2.Joint = jnt2;
body3.Joint = jnt3;
body4.Joint = jnt4;
body5.Joint = jnt5;
body6.Joint = jnt6;
body7.Joint = jnt7;
body8.Joint = jnt8;
body9.Joint = jnt9;
body10.Joint = jnt10;
body11.Joint = jnt11;
body12.Joint = jnt12;

addBody(robot,body2,'body1')
addBody(robot,body3,'body2')
addBody(robot,body4,'body3')
addBody(robot,body5,'body4')
addBody(robot,body6,'body5')
addBody(robot,body7,'body6')
addBody(robot,body8,'body7')
addBody(robot,body9,'body8')
addBody(robot,body10,'body9')
addBody(robot,body11,'body10')
addBody(robot,body12,'body11')


end

