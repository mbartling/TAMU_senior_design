%% A MonteCarlo Simulation of Yonderway over several different iteration amounts

%%
NumTest = 100;
alphaMax = 20; % Arbitruary
geometricR = 0.8; %dont feel like changing
pathNoiseABSMax = 200;
TrialRuns = zeros(1,8);
for i = 3:5
    for j = 1:100
        TestCase.alpha        = randi(alphaMax, 1);         % A higher alpha seems to work better
        TestCase.geometricR   = geometricR;    % must be greater than 0.5
        TestCase.numRuns      = i;       % 3 is the minimum useful number
        TestCase.pathNoiseMax = randi(pathNoiseABSMax,1);  % minimum noise = 1, max = some giant
        Trial = ProfileYonderway_Simple( TestCase );
        TrialRuns = [TrialRuns; Trial.TrialParams,Trial.TrialCloseness];
    end
end
TrialRuns = TrialRuns(2:end,:);
importantStuff = TrialRuns(:,[1,3,4,5,6,7,8]);
% close_g1
% close_g2
% close_g3
% close_b1
% alpha
% geometricR 
% numRuns
% pathNoiseMax

Iam = 'Done'