int A = A4;
int B = A5;
int C = 2;
int D = 3;
int E = 4;
int F = 5;
int G = 6;
int H = 7;
int I = 8;
int J = 9;
int K = 10;
int L = 11;
int M = 12;
int N = 13;

int CHANNEL_COUNT = 14;

int channels[] = 
{
  A, B, C, D, E, F, G, H, I, J, K, L, M, N
};

int i = 0;
int incomingByte[14];
int BAUD_RATE = 57600;

void setup()
{
  Serial.begin(BAUD_RATE);

  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    pinMode(channels[i], OUTPUT);  
  }
}

void loop()
{
  if (Serial.available() >= CHANNEL_COUNT)
  {
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }                                  
    
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      digitalWrite(channels[i], incomingByte[i]);
    }
  }
}
