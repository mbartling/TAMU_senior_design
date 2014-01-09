function [ API_Frame ] = makeTxFrame( apiFrameIdentifier, FrameID, DstAddr, OptionsByte, Data )
%makeFrame Summary of this function goes here
%   Detailed explanation goes here
API_Frame.apiFrameIdentifier = apiFrameIdentifier;
API_Frame.FrameID            = FrameID;
API_Frame.DstAddr            = DstAddr;
API_Frame.OptionsByte        = OptionsByte;
API_Frame.Data               = Data;

end

