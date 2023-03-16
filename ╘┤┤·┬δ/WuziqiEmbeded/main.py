import serial
import random
import time

USART1 = serial.Serial(port='COM8', baudrate='9600')

#covert string to inter list, e.g. '123' -> [1,2,3]
def String2Array(data):
    data1 = [(ord(c)-48) for c in data]
    return data1

ChekerBoard = []
def InitCheckerBoard():
    a = []
    for i in range(0, 10):
        a.append(0)
    for i in range(0, 10):
        ChekerBoard.append(a)

def ComputerPlayChess(count):
    '''
    i = random.randint(0, 9)
    j = random.randint(0, 9)
    while ChekerBoard[i][j]:
        i = random.randint(0, 9)
        j = random.randint(0, 9)
    return [i, j]
    '''
    i = int(count/10)
    j = count % 10
    return [i, j]


if __name__ == "__main__":
    count = 0
    #SendData = b'123456789123456789999\r\n'
    #USART1.open()
    #a = USART1.write(SendData)
    InitCheckerBoard()

    while True:
        ReceiveData = USART1.read(2)#read at most 128 bytes
        ReceiveString = ReceiveData.decode()
        Coordinate = String2Array(ReceiveString)
        ChekerBoard[Coordinate[0]][Coordinate[1]] = 1
        print("Player put chess at: ", Coordinate)

        time.sleep(2)


        step = ComputerPlayChess(count)
        ChekerBoard[step[0]][step[1]] = 2
        print("computer step at: ", step)
        SendData = step[0].to_bytes(1, 'little') + step[1].to_bytes(1, 'little') + b'\r\n'
        USART1.write(SendData)

        count += 1
    USART1.close()