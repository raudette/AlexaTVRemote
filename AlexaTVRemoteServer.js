var WebSocketServer = require('ws').Server
const express = require('express')
const app = express()
const { ExpressAdapter } = require('ask-sdk-express-adapter');
const Alexa = require('ask-sdk-core')
const skillBuilder = Alexa.SkillBuilders.custom();


wss = new WebSocketServer({port: 8080})
CLIENTS=[];

wss.on('connection', function(ws) {
    CLIENTS.push(ws);
});

function sendAll (message) {
    for (var i=0; i<CLIENTS.length; i++) {
        CLIENTS[i].send("Message: " + message);
    }
}

const LaunchRequestHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'LaunchRequest';
  },
  handle(handlerInput) {
      const speechText = 'Launching Toshiba TV Skill';

      return handlerInput.responseBuilder
          .speak(speechText)
          .reprompt(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

const CancelAndStopIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && (handlerInput.requestEnvelope.request.intent.name === 'AMAZON.CancelIntent'
              || handlerInput.requestEnvelope.request.intent.name === 'AMAZON.StopIntent');
  },
  handle(handlerInput) {
      const speechText = 'Goodbye!';

      return handlerInput.responseBuilder
          .speak(speechText)
          .withSimpleCard('Hello World', speechText)
          .withShouldEndSession(true)
          .getResponse();
  }
};

const HelpIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.HelpIntent';
  },
  handle(handlerInput) {
      const speechText = 'You can say power on, power off, volume up, volume down, and mute.';

      return handlerInput.responseBuilder
          .speak(speechText)
          .reprompt(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

const ErrorHandler = {
  canHandle() {
      return true;
  },
  handle(handlerInput, error) {
      console.log(`Error handled: ${error.message}`);

      return handlerInput.responseBuilder
          .speak('Sorry, I can\'t understand the command. Please say again.')
          .reprompt('Sorry, I can\'t understand the command. Please say again.')
          .getResponse();
  },
};

const EntertainmeIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && handlerInput.requestEnvelope.request.intent.name === 'entertainme';
  },
  handle(handlerInput) {
      const speechText = 'Toggling power';
      sendAll("power")  

      return handlerInput.responseBuilder
          .speak(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

const volupIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && handlerInput.requestEnvelope.request.intent.name === 'volup';
  },
  handle(handlerInput) {
      const speechText = 'Turning volume up';
      sendAll("volup")  

      return handlerInput.responseBuilder
          .speak(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

const voldownIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && handlerInput.requestEnvelope.request.intent.name === 'voldown';
  },
  handle(handlerInput) {
      const speechText = 'Turning volume down';
      sendAll("voldown")  
      return handlerInput.responseBuilder
          .speak(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

const muteIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
          && handlerInput.requestEnvelope.request.intent.name === 'mute';
  },
  handle(handlerInput) {
      const speechText = 'Mute';
      sendAll("mute")  

      return handlerInput.responseBuilder
          .speak(speechText)
          .withSimpleCard('Hello World', speechText)
          .getResponse();
  }
};

skillBuilder.addRequestHandlers(
  LaunchRequestHandler,
  CancelAndStopIntentHandler,
  HelpIntentHandler,
  EntertainmeIntentHandler,
  volupIntentHandler,
  voldownIntentHandler,
  muteIntentHandler
)

skillBuilder.addErrorHandler(
  ErrorHandler
)

const skill = skillBuilder.create();

const adapter = new ExpressAdapter(skill, false, false);

app.post('/', adapter.getRequestHandlers());
 

app.get('/power', function (req, res) {
    sendAll("power")  
    res.send('Power')
  })

app.get('/volup', function (req, res) {
    sendAll("volup")  
    res.send('volup')
  })

app.get('/voldown', function (req, res) {
    sendAll("voldown")  
    res.send('voldown')
  })
    

app.listen(3000)
