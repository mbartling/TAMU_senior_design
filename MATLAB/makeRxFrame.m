function [ API_Frame ] = makeRxFrame( apiFrameIdentifier, FrameID, SrcAddr, OptionsByte, Data )
%makeRxFrame Summary of this function goes here
%   Detailed explanation goes here
API_Frame.apiFrameIdentifier = apiFrameIdentifier;
API_Frame.FrameID            = FrameID;
API_Frame.DstAddr            = SrcAddr;
API_Frame.OptionsByte        = uint8(0);
API_Frame.Data               = Data;

end

