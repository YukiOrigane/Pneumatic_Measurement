
addpath('func')

% --- Serial settings ---
% port = "COM5";      % 適宜変更
% baud = 115200;      % ユーザー希望の115200bps
% s = serialport(port, baud);
% 
% configureTerminator(s, "CR/LF");  % 0x13 0x10 を終端とみなす
% flush(s);


%% 送信
% --- 送信したいデータ ---
data_tx = [3391, -3102];    % d1, d2 は -10000 ～ 10000 の整数

% --- 送信バイト列の作成 ---
tx_bytes = zeros(1,1+3*length(data_tx)+2-1);

% 開始コード '!' (0x21)
tx_bytes(1) = uint8(0x21);

for k = 1:length(data_tx)
    [hi, lo] = encodeData(data_tx(k));
    tx_bytes(1+3*k-2) = hi;
    tx_bytes(1+3*k-1) = lo;

    % データ区切り（最後は入れても入れなくても良い）
    if k < length(data_tx)
        tx_bytes(1+3*k) = uint8(0x20); % space
    end
end

% 終端 CR LF
tx_bytes(end-1) = uint8(0x13);
tx_bytes(end) = uint8(0x10);

% --- 実際の送信 ---
%write(s, tx_bytes, "uint8");

%% 受信
% --- 受信待ち ---
%raw = readuntil(s, "CR/LF");  % CR LF まで読む
raw = tx_bytes;
bytes = uint8(raw);

% --- パケット解析 ---
if bytes(1) ~= uint8(0x21)
    error("Start byte invalid.");
end

% 「!」を除く
bytes = bytes(2:end);

% CR(0x13) と LF(0x10) を除去
bytes(bytes == 0x13 | bytes == 0x10) = [];

% Space(0x20) で区切る
tokens = {};
buf = [];
for i = 1:length(bytes)
    if bytes(i) == 0x20
        tokens{end+1} = buf; %#ok
        buf = [];
    else
        buf(end+1) = bytes(i);
    end
end
if ~isempty(buf)
    tokens{end+1} = buf;  % 最後のデータ
end

% --- デコード ---
data_rx = zeros(1, length(tokens));
for i = 1:length(tokens)
    b = tokens{i};
    if length(b) ~= 2
        error("Received data length error");
    end
    data_rx(i) = decodeData(b(1), b(2));
end

disp("Received:");
disp(data_rx);





