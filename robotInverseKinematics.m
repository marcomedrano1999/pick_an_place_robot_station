function [configSoln, solnInfo] = robotInverseKinematics(robot, tform)


%% Compute inverse kinematics
ik = inverseKinematics('RigidBodyTree',robot);
weights = [0.25 0.25 0.25 1 1 1];
initialguess = robot.homeConfiguration;
[configSoln,solnInfo] = ik('gripper',tform,weights,initialguess);
configSoln = wrapToPi(configSoln);
end

