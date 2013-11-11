function [ packet ] = Packetize( FrameData )
%% PACKETIZE Take the input data and form a packet
%    IEEE 802.15.4 packet structure
%    
    packet.startDelimeter = uint8(hex2dec('7E'));
    packet.packetLength = typecast(swapbytes(uint16(calcFrameLength(FrameData))),'uint8');
    packet.FrameData = FrameData;
    packet.checksum = doChecksum(FrameData);
    

end

