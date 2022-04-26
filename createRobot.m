function robot = createRobot()
%% Define links lenghts
LENGHT_JNT_1 = 0.95;
LENGHT_JNT_2 = 1.05;
LENGHT_JNT_3 = 1.3;
LENGHT_JNT_4 = 0.75;
LENGHT_GRIPPER = 1.05;

%% build model
robot = rigidBodyTree("DataFormat","column");
base = robot.Base;

%% Add links
rotatingBase = rigidBody("rotating_base");
arm1 = rigidBody("arm1");
arm2 = rigidBody("arm2");
arm3 = rigidBody("arm3");
arm4 = rigidBody("arm4");
gripper = rigidBody("gripper");

collBase = collisionCylinder(0.25,LENGHT_JNT_1); % cylinder: radius,length
collBase.Pose = trvec2tform([0 0 LENGHT_JNT_1/2]);
coll1 = collisionBox(0.1,0.1,LENGHT_JNT_2); % box: length, width, height (x,y,z)
coll1.Pose = trvec2tform([0 0 LENGHT_JNT_2/2]);
coll2 = collisionBox(0.1,0.1,LENGHT_JNT_3); % box: length, width, height (x,y,z)
coll2.Pose = trvec2tform([0 0 LENGHT_JNT_3/2]);
coll3 = collisionBox(0.1,0.1,LENGHT_JNT_4); % box: length, width, height (x,y,z)
coll3.Pose = trvec2tform([0 0 LENGHT_JNT_4/2]);
coll4 = collisionBox(0.1,0.1,LENGHT_GRIPPER); % box: length, width, height (x,y,z)
coll4.Pose = trvec2tform([0 0 LENGHT_GRIPPER/2]);
collGripper = collisionSphere(0.125); %sphere: radius
collGripper.Pose = trvec2tform([0 -0.015 0.025/2]);

addCollision(rotatingBase,collBase)
addCollision(arm1,coll1)
addCollision(arm2,coll2)
addCollision(arm3,coll3)
addCollision(arm4,coll4)
addCollision(gripper,collGripper)

%% Add joints
jntBase = rigidBodyJoint("base_joint","revolute");
jnt1 = rigidBodyJoint("jnt1","revolute");
jnt2 = rigidBodyJoint("jnt2","revolute");
jnt3 = rigidBodyJoint("jnt3","revolute");
jnt4 = rigidBodyJoint("jnt4","revolute");
jntGripper = rigidBodyJoint("gripper_joint","revolute");

jnt1.JointAxis = [1 0 0]; % x-axis
jnt2.JointAxis = [1 0 0];
jnt3.JointAxis = [1 0 0];
jntGripper.JointAxis = [0 1 0]; % y-axis

setFixedTransform(jnt1,trvec2tform([0 0 LENGHT_JNT_1]))
setFixedTransform(jnt2,trvec2tform([0 0 LENGHT_JNT_2]))
setFixedTransform(jnt3,trvec2tform([0 0 LENGHT_JNT_3]))
setFixedTransform(jnt4,trvec2tform([0 0 LENGHT_JNT_4]))
setFixedTransform(jntGripper,trvec2tform([0 0 LENGHT_GRIPPER]))

%% Assembly robot

bodies = {base,rotatingBase,arm1,arm2,arm3,arm4,gripper};
joints = {[],jntBase,jnt1,jnt2,jnt3,jnt4,jntGripper};

% figure("Name","Assemble Robot","Visible","on")
for i = 2:length(bodies) % Skip base. Iterate through adding bodies and joints.
            bodies{i}.Joint = joints{i};
            addBody(robot,bodies{i},bodies{i-1}.Name)
%             show(robot,"Collisions","on","Frames","off");
%             drawnow;
end

end

