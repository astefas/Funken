 void FunkenSetupImplementEssentials() {
  fnk.listenTo("CLIST", funkenListComands); 
  fnk.listenTo("V", funkenVerbose);
  fnk.listenTo("SETID", funkenSetID);
  fnk.listenTo("GETID", funkenGetID);

  fnk.verbosed(funkenVerboseOutput);
  fnk.unhandled(funkenUnhandledSerialInput);
}

void funkenSetID(char *c) {
  char *token = fnk.getToken(c);
  char *value = fnk.getArgument(c);

  int valueint = atoi(value);
  
  fnk.id = valueint;
}

void funkenGetID(char *c) {
  char *token = fnk.getToken(c);

  fnk.respond(token, fnk.id); 
}

/*
 * TYPICAL: COMANDLIST [NO ATTRIBUTES]
 */
void funkenListComands(char *c) {
  char *token = fnk.getToken(c);

  String commandList = "";
  boolean startHasBeenTokenized = false;
  for(int i = 0; i < fnk.numOfCallbacks; i++){
    if(startHasBeenTokenized) commandList += F(" ");
    String command = String(fnk.callbacks[i].com);
    
    if(command != String(token)) {
      commandList += String(fnk.callbacks[i].com);

      if(!startHasBeenTokenized) startHasBeenTokenized = true;
    }
  }

  fnk.respond(token, String(commandList));
}

/*
 * TYPICAL: V 1 or V [1 or 0]
 */
void funkenVerbose(char *c) {
  char *token = fnk.getToken(c);
  char *value = fnk.getArgument(c);

  int valueint = atoi(value);
  fnk.verbose = valueint;
}

/*
 * TYPICAL: SOMETHING NOT IN THE COMANDLIST
 */
void funkenUnhandledSerialInput(char *c) {
  fnk.respond(c, F("!H"));
}

/*
 * TYPICAL: SOMETHING NOT IN THE COMANDLIST
 */
void funkenVerboseOutput(char *c) {
  fnk.respond(c, F("H"));
}
