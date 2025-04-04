with open('./test/simple_add', 'wb') as f:
    # magic
    f.write(b'\xFE\xFA\xFA\xFC')
    # 8 bytes in data
    f.write(b'\x00\x08')
    # literal 5
    f.write(b'\x00\x00\x00\x05')
    # literal 8
    f.write(b'\x00\x00\x00\x03')
    # instructions length
    f.write(b'\x00\x08')
    # instructions
    # PUSH DATA[0] to DATA[4]
    f.write(b'\x06\xE0\x40')
    # PUSH DATA[4] to DATA[8]
    f.write(b'\x06\xE0\x44')
    # ADD
    f.write(b'\x02')
    # PRINT top of stack
    f.write(b'\x08')
    # TERMINATING ZERO
    f.write(b'\x00')
