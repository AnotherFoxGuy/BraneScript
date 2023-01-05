
// Generated from C:/Users/eli/Documents/GitHub/BraneScript/antlr4\brane.g4 by ANTLR 4.11.1


#include "braneLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct BraneLexerStaticData final {
  BraneLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  BraneLexerStaticData(const BraneLexerStaticData&) = delete;
  BraneLexerStaticData(BraneLexerStaticData&&) = delete;
  BraneLexerStaticData& operator=(const BraneLexerStaticData&) = delete;
  BraneLexerStaticData& operator=(BraneLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag branelexerLexerOnceFlag;
BraneLexerStaticData *branelexerLexerStaticData = nullptr;

void branelexerLexerInitialize() {
  assert(branelexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<BraneLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "T__22", "T__23", "T__24", 
      "T__25", "T__26", "T__27", "NEWLINE", "COMMENT", "BLOCK_COMMENT", 
      "SPACE", "BOOL", "INT", "FLOAT", "CHAR", "STRING", "ID", "MUL", "DIV", 
      "ADD", "SUB"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'const'", "'ref'", "','", "'('", "')'", "'{'", "'}'", "'#include'", 
      "'link'", "'as'", "';'", "'public'", "'packed'", "'struct'", "'return'", 
      "'if'", "'while'", "'delete'", "'.'", "'['", "']'", "'=='", "'!='", 
      "'<'", "'>'", "'<='", "'>='", "'='", "", "", "", "", "", "", "", "", 
      "", "", "'*'", "'/'", "'+'", "'-'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "NEWLINE", "COMMENT", 
      "BLOCK_COMMENT", "SPACE", "BOOL", "INT", "FLOAT", "CHAR", "STRING", 
      "ID", "MUL", "DIV", "ADD", "SUB"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,42,286,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,1,0,
  	1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,
  	6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,1,9,1,9,
  	1,9,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,
  	1,12,1,12,1,12,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,14,1,14,1,14,1,14,
  	1,14,1,14,1,14,1,15,1,15,1,15,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,
  	1,17,1,17,1,17,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,21,
  	1,22,1,22,1,22,1,23,1,23,1,24,1,24,1,25,1,25,1,25,1,26,1,26,1,26,1,27,
  	1,27,1,28,4,28,194,8,28,11,28,12,28,195,1,28,1,28,1,29,1,29,1,29,1,29,
  	5,29,204,8,29,10,29,12,29,207,9,29,1,29,1,29,1,29,1,29,1,30,1,30,1,30,
  	1,30,5,30,217,8,30,10,30,12,30,220,9,30,1,30,1,30,1,30,1,30,1,30,1,31,
  	1,31,1,31,1,31,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,1,32,3,32,240,
  	8,32,1,33,4,33,243,8,33,11,33,12,33,244,1,34,1,34,1,34,5,34,250,8,34,
  	10,34,12,34,253,9,34,3,34,255,8,34,1,34,1,34,1,35,1,35,1,35,1,35,1,36,
  	1,36,5,36,265,8,36,10,36,12,36,268,9,36,1,36,1,36,1,37,1,37,5,37,274,
  	8,37,10,37,12,37,277,9,37,1,38,1,38,1,39,1,39,1,40,1,40,1,41,1,41,3,205,
  	218,266,0,42,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,
  	25,13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,21,43,22,45,23,47,
  	24,49,25,51,26,53,27,55,28,57,29,59,30,61,31,63,32,65,33,67,34,69,35,
  	71,36,73,37,75,38,77,39,79,40,81,41,83,42,1,0,6,2,0,10,10,13,13,5,0,10,
  	10,13,13,40,41,69,70,79,79,2,0,9,9,32,32,1,0,48,57,3,0,65,90,95,95,97,
  	122,4,0,48,57,65,90,95,95,97,122,294,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,
  	0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,
  	17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,
  	0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,
  	0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,
  	49,1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,
  	0,0,0,0,61,1,0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,
  	0,0,71,1,0,0,0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,
  	81,1,0,0,0,0,83,1,0,0,0,1,85,1,0,0,0,3,91,1,0,0,0,5,95,1,0,0,0,7,97,1,
  	0,0,0,9,99,1,0,0,0,11,101,1,0,0,0,13,103,1,0,0,0,15,105,1,0,0,0,17,114,
  	1,0,0,0,19,119,1,0,0,0,21,122,1,0,0,0,23,124,1,0,0,0,25,131,1,0,0,0,27,
  	138,1,0,0,0,29,145,1,0,0,0,31,152,1,0,0,0,33,155,1,0,0,0,35,161,1,0,0,
  	0,37,168,1,0,0,0,39,170,1,0,0,0,41,172,1,0,0,0,43,174,1,0,0,0,45,177,
  	1,0,0,0,47,180,1,0,0,0,49,182,1,0,0,0,51,184,1,0,0,0,53,187,1,0,0,0,55,
  	190,1,0,0,0,57,193,1,0,0,0,59,199,1,0,0,0,61,212,1,0,0,0,63,226,1,0,0,
  	0,65,239,1,0,0,0,67,242,1,0,0,0,69,246,1,0,0,0,71,258,1,0,0,0,73,262,
  	1,0,0,0,75,271,1,0,0,0,77,278,1,0,0,0,79,280,1,0,0,0,81,282,1,0,0,0,83,
  	284,1,0,0,0,85,86,5,99,0,0,86,87,5,111,0,0,87,88,5,110,0,0,88,89,5,115,
  	0,0,89,90,5,116,0,0,90,2,1,0,0,0,91,92,5,114,0,0,92,93,5,101,0,0,93,94,
  	5,102,0,0,94,4,1,0,0,0,95,96,5,44,0,0,96,6,1,0,0,0,97,98,5,40,0,0,98,
  	8,1,0,0,0,99,100,5,41,0,0,100,10,1,0,0,0,101,102,5,123,0,0,102,12,1,0,
  	0,0,103,104,5,125,0,0,104,14,1,0,0,0,105,106,5,35,0,0,106,107,5,105,0,
  	0,107,108,5,110,0,0,108,109,5,99,0,0,109,110,5,108,0,0,110,111,5,117,
  	0,0,111,112,5,100,0,0,112,113,5,101,0,0,113,16,1,0,0,0,114,115,5,108,
  	0,0,115,116,5,105,0,0,116,117,5,110,0,0,117,118,5,107,0,0,118,18,1,0,
  	0,0,119,120,5,97,0,0,120,121,5,115,0,0,121,20,1,0,0,0,122,123,5,59,0,
  	0,123,22,1,0,0,0,124,125,5,112,0,0,125,126,5,117,0,0,126,127,5,98,0,0,
  	127,128,5,108,0,0,128,129,5,105,0,0,129,130,5,99,0,0,130,24,1,0,0,0,131,
  	132,5,112,0,0,132,133,5,97,0,0,133,134,5,99,0,0,134,135,5,107,0,0,135,
  	136,5,101,0,0,136,137,5,100,0,0,137,26,1,0,0,0,138,139,5,115,0,0,139,
  	140,5,116,0,0,140,141,5,114,0,0,141,142,5,117,0,0,142,143,5,99,0,0,143,
  	144,5,116,0,0,144,28,1,0,0,0,145,146,5,114,0,0,146,147,5,101,0,0,147,
  	148,5,116,0,0,148,149,5,117,0,0,149,150,5,114,0,0,150,151,5,110,0,0,151,
  	30,1,0,0,0,152,153,5,105,0,0,153,154,5,102,0,0,154,32,1,0,0,0,155,156,
  	5,119,0,0,156,157,5,104,0,0,157,158,5,105,0,0,158,159,5,108,0,0,159,160,
  	5,101,0,0,160,34,1,0,0,0,161,162,5,100,0,0,162,163,5,101,0,0,163,164,
  	5,108,0,0,164,165,5,101,0,0,165,166,5,116,0,0,166,167,5,101,0,0,167,36,
  	1,0,0,0,168,169,5,46,0,0,169,38,1,0,0,0,170,171,5,91,0,0,171,40,1,0,0,
  	0,172,173,5,93,0,0,173,42,1,0,0,0,174,175,5,61,0,0,175,176,5,61,0,0,176,
  	44,1,0,0,0,177,178,5,33,0,0,178,179,5,61,0,0,179,46,1,0,0,0,180,181,5,
  	60,0,0,181,48,1,0,0,0,182,183,5,62,0,0,183,50,1,0,0,0,184,185,5,60,0,
  	0,185,186,5,61,0,0,186,52,1,0,0,0,187,188,5,62,0,0,188,189,5,61,0,0,189,
  	54,1,0,0,0,190,191,5,61,0,0,191,56,1,0,0,0,192,194,7,0,0,0,193,192,1,
  	0,0,0,194,195,1,0,0,0,195,193,1,0,0,0,195,196,1,0,0,0,196,197,1,0,0,0,
  	197,198,6,28,0,0,198,58,1,0,0,0,199,200,5,47,0,0,200,201,5,47,0,0,201,
  	205,1,0,0,0,202,204,9,0,0,0,203,202,1,0,0,0,204,207,1,0,0,0,205,206,1,
  	0,0,0,205,203,1,0,0,0,206,208,1,0,0,0,207,205,1,0,0,0,208,209,7,1,0,0,
  	209,210,1,0,0,0,210,211,6,29,0,0,211,60,1,0,0,0,212,213,5,47,0,0,213,
  	214,5,42,0,0,214,218,1,0,0,0,215,217,9,0,0,0,216,215,1,0,0,0,217,220,
  	1,0,0,0,218,219,1,0,0,0,218,216,1,0,0,0,219,221,1,0,0,0,220,218,1,0,0,
  	0,221,222,5,42,0,0,222,223,5,47,0,0,223,224,1,0,0,0,224,225,6,30,0,0,
  	225,62,1,0,0,0,226,227,7,2,0,0,227,228,1,0,0,0,228,229,6,31,0,0,229,64,
  	1,0,0,0,230,231,5,116,0,0,231,232,5,114,0,0,232,233,5,117,0,0,233,240,
  	5,101,0,0,234,235,5,102,0,0,235,236,5,97,0,0,236,237,5,108,0,0,237,238,
  	5,115,0,0,238,240,5,101,0,0,239,230,1,0,0,0,239,234,1,0,0,0,240,66,1,
  	0,0,0,241,243,7,3,0,0,242,241,1,0,0,0,243,244,1,0,0,0,244,242,1,0,0,0,
  	244,245,1,0,0,0,245,68,1,0,0,0,246,254,3,67,33,0,247,251,5,46,0,0,248,
  	250,7,3,0,0,249,248,1,0,0,0,250,253,1,0,0,0,251,249,1,0,0,0,251,252,1,
  	0,0,0,252,255,1,0,0,0,253,251,1,0,0,0,254,247,1,0,0,0,254,255,1,0,0,0,
  	255,256,1,0,0,0,256,257,5,102,0,0,257,70,1,0,0,0,258,259,5,39,0,0,259,
  	260,9,0,0,0,260,261,5,39,0,0,261,72,1,0,0,0,262,266,5,34,0,0,263,265,
  	9,0,0,0,264,263,1,0,0,0,265,268,1,0,0,0,266,267,1,0,0,0,266,264,1,0,0,
  	0,267,269,1,0,0,0,268,266,1,0,0,0,269,270,5,34,0,0,270,74,1,0,0,0,271,
  	275,7,4,0,0,272,274,7,5,0,0,273,272,1,0,0,0,274,277,1,0,0,0,275,273,1,
  	0,0,0,275,276,1,0,0,0,276,76,1,0,0,0,277,275,1,0,0,0,278,279,5,42,0,0,
  	279,78,1,0,0,0,280,281,5,47,0,0,281,80,1,0,0,0,282,283,5,43,0,0,283,82,
  	1,0,0,0,284,285,5,45,0,0,285,84,1,0,0,0,10,0,195,205,218,239,244,251,
  	254,266,275,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  branelexerLexerStaticData = staticData.release();
}

}

braneLexer::braneLexer(CharStream *input) : Lexer(input) {
  braneLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *branelexerLexerStaticData->atn, branelexerLexerStaticData->decisionToDFA, branelexerLexerStaticData->sharedContextCache);
}

braneLexer::~braneLexer() {
  delete _interpreter;
}

std::string braneLexer::getGrammarFileName() const {
  return "brane.g4";
}

const std::vector<std::string>& braneLexer::getRuleNames() const {
  return branelexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& braneLexer::getChannelNames() const {
  return branelexerLexerStaticData->channelNames;
}

const std::vector<std::string>& braneLexer::getModeNames() const {
  return branelexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& braneLexer::getVocabulary() const {
  return branelexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView braneLexer::getSerializedATN() const {
  return branelexerLexerStaticData->serializedATN;
}

const atn::ATN& braneLexer::getATN() const {
  return *branelexerLexerStaticData->atn;
}




void braneLexer::initialize() {
  ::antlr4::internal::call_once(branelexerLexerOnceFlag, branelexerLexerInitialize);
}
