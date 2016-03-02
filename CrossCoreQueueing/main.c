/*
 * main.c
 *
 * that's top be stored on GitHub for training
 * another nice comment to be pushed upstream
 *
 *  Created on: 01.03.2016
 *      Author: Dietmar
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define Q_SLOTS   (4)

static volatile uint16_t queue[Q_SLOTS];

static volatile uint8_t rdIdx = 0x00;

// rdIdx == wrIdx --> queue empty; idxDiff = 1 --> queue full

static volatile uint8_t wrIdx = 0x00;

void tx_core(uint16_t txVal)
{
  printf("TX - %04x -> ",txVal);

  {
    uint32_t rdIdxCpy = rdIdx; // shot a snap of the read index
    uint32_t empty = (rdIdxCpy > wrIdx) ? rdIdxCpy - wrIdx - 1 : (Q_SLOTS-1) - wrIdx + rdIdxCpy;

    if (empty >= 1)
    {
      uint8_t currWrIdx = wrIdx;
      queue[currWrIdx] = txVal;

      if (currWrIdx < (Q_SLOTS-1))
      {
        wrIdx = currWrIdx+1;
      }
      else
      {
        wrIdx=0;
      }
      printf(" @ %d (empty: %d  rdIdxCpy: %d wrIdx: %d)\n",currWrIdx,empty,rdIdxCpy,wrIdx);
    }
    else
    {
      printf(" BLOCKED (empty: %d)\n",empty);
    }
  }
}

#define DEQUEUEING_TRIGGER_OPTION     (0xA)      // 0xA or 0xB


void rx_core(void)
{
  uint16_t rxVal;

  {
    static uint32_t lastWrIdx = 0;

    uint32_t wrIdxCpy = wrIdx; // shot a snap of the write index

    uint32_t filled;

#if DEQUEUEING_TRIGGER_OPTION == 0xA
    /* opt. A: queue filling changed, if write index has moved */
    filled = (wrIdxCpy != lastWrIdx) ? 1 : 0;
    lastWrIdx = wrIdxCpy;

#else
    /* opt. B: calculate exact queue filling */
    uint32_t empty = (rdIdx > wrIdxCpy) ? rdIdx - wrIdxCpy - 1 : (Q_SLOTS-1) - wrIdxCpy + rdIdx;
    filled = Q_SLOTS - empty - 1;
#endif

    if (filled >= 1)
    {
      uint8_t currRdIdx = rdIdx;
      rxVal = queue[currRdIdx];

      if (currRdIdx < (Q_SLOTS-1))
      {
        rdIdx = currRdIdx+1;
      }
      else
      {
        rdIdx=0;
      }
      printf("- %04x ->RX from %d (new rdIdx: %d)\n",rxVal,currRdIdx,rdIdx);
    }
    else
    {
      printf(" EMPTY\n");
    }
  }
}

/****************************************************************************************/

int main(void)
{

// Using NDEBUG as also used by <assert.h>; feel free to use a different define
#ifndef NDEBUG
  // For debugging, set stdout to unbuffered
  setbuf( stdout, NULL );
#endif

  {
    uint32_t i;
    for (i=0; i<Q_SLOTS; i++)
      queue[i]=0;
  }

  printf("Hello World !\n\n");

  tx_core(0xA001);
  tx_core(0xA002);
  tx_core(0xA003);
  tx_core(0xA004);
  tx_core(0xA005);
  tx_core(0xA006);

  rx_core();
  rx_core();
  rx_core();
  rx_core();
  rx_core();
  rx_core();

  tx_core(0xA004);
  tx_core(0xA005);
  tx_core(0xA006);
  tx_core(0xA007);
  tx_core(0xA008);

  rx_core();
  rx_core();
  rx_core();
  rx_core();
  rx_core();
  rx_core();

  tx_core(0xA009);
  tx_core(0xA010);

  rx_core();
  rx_core();
  rx_core();
  rx_core();

  tx_core(0xA020);
  rx_core();
  tx_core(0xA021);
  rx_core();
  tx_core(0xA022);
  rx_core();
  tx_core(0xA023);
  rx_core();
  tx_core(0xA024);
  rx_core();
  tx_core(0xA025);
  rx_core();
  tx_core(0xA026);
  rx_core();
  tx_core(0xA027);
  rx_core();
  tx_core(0xA028);
  rx_core();
  tx_core(0xA029);
  rx_core();
  tx_core(0xA030);
  rx_core();
  tx_core(0xA031);
  rx_core();
  tx_core(0xA032);
  rx_core();
  tx_core(0xA033);
  rx_core();

  exit(0);
}





