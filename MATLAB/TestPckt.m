DstAddr = uint8(zeros(8,1));
%DstAddr(5:8) 
DstAddr(5:8) = typecast(uint8([hex2dec('C0') hex2dec('A8') hex2dec('00') hex2dec('64') ]),'uint8');
DstAddr = DstAddr';
FrameID = uint8(1);
OptionsByte = uint8(0);
Data = typecast(hex2dec('1516'),'uint8');
Data = Data(6:8);

APIFrame = makeTxFrame(uint8(0), FrameID, DstAddr, OptionsByte, Data);
structfun(@(field)length(field), APIFrame);
pckt = Packetize(APIFrame)
dec2hex(pckt.checksum)