/*
 *
 *  Created on: 16 févr. 2018
 *      Author: thomas
 */

#ifndef CODEBOOK_H_
#define CODEBOOK_H_
#include "data_structures.h"

//data codebook

#include "codebook0_speaker.dat"
#include "codebook1_speaker.dat"
#include "codebook2_speaker.dat"
#include "codebook3_speaker.dat"
#include "codebook4_speaker.dat"
#include "codebook5_speaker.dat"
#include "codebook6_speaker.dat"
#include "codebook7_speaker.dat"
#include "codebook8_speaker.dat"
#include "codebook9_speaker.dat"
#include "codebookA_speaker.dat"
#include "codebookB_speaker.dat"
#include "codebookC_speaker.dat"
#include "codebookD_speaker.dat"

#include "codebook0_animal.dat"
#include "codebook1_animal.dat"
#include "codebook2_animal.dat"
#include "codebook3_animal.dat"
#include "codebook4_animal.dat"
#include "codebook5_animal.dat"
#include "codebook6_animal.dat"
#include "codebook7_animal.dat"
#include "codebook8_animal.dat"
#include "codebook9_animal.dat"
#include "codebookA_animal.dat"
#include "codebookB_animal.dat"
#include "codebookC_animal.dat"
#include "codebookD_animal.dat"

#include "codebook0_phoneme.dat"
#include "codebook1_phoneme.dat"
#include "codebook2_phoneme.dat"
#include "codebook3_phoneme.dat"
#include "codebook4_phoneme.dat"
#include "codebook5_phoneme.dat"
#include "codebook6_phoneme.dat"
#include "codebook7_phoneme.dat"
#include "codebook8_phoneme.dat"
#include "codebook9_phoneme.dat"
#include "codebookA_phoneme.dat"
#include "codebookB_phoneme.dat"
#include "codebookC_phoneme.dat"
#include "codebookD_phoneme.dat"


#pragma DATA_SECTION(codebook0_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook1_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook2_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook3_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook4_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook5_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook6_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook7_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook8_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebook9_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebookA_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebookB_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebookC_phoneme, ".EXT_RAM")
#pragma DATA_SECTION(codebookD_phoneme, ".EXT_RAM")


char *filename[SPEAKER_NB_MAX] = {"codebook0",
                                  "codebook1",
                                  "codebook2",
                                  "codebook3",
                                  "codebook4",
                                  "codebook5",
                                  "codebook6",
                                  "codebook7",
                                  "codebook8",
                                  "codebook9",
                                  "codebookA",
                                  "codebookB",
                                  "codebookC",
                                  "codebookD"};


#endif 
