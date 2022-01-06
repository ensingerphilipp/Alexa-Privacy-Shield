**web-voice-record** is small web app to collect speech samples and upload them to Google cloud storage.  Thank you to Pete Warden for the Open Speech Recording application (https://github.com/petewarden/open-speech-recording) that I used as a starting point.

This is a small Flask app that runs on Google App Engine. It serves up a client-side Javascript app that prompts for words to be uttered, recorded, and posted to a Google cloud storage bucket.  This app uses the microphone and require HTML5 support on the client device.  

For use in collecting speech recordings from friends and family to detect the keyphrase 'Friday*', I modified Pete Wardens original work in following ways:

1. Modified text/message in all templates (German) (original: https://github.com/petewarden/open-speech-recording/blob/master/templates)
2. Modified var wantedWords and var fillerWords in app.js (original: https://github.com/petewarden/open-speech-recording/blob/master/static/scripts/app.js)

## Running
Please follow the directions provided by Pete Warden located here (https://github.com/petewarden/open-speech-recording/edit/master/README.md)

## Credits
Thanks to Pete Warden for the Open Speech Recording application (https://github.com/petewarden/open-speech-recording)

Pete also credited:
'Thanks to the Mozilla team for the Web Dictaphone sample application that I used as a starting point, Sole for the oscilloscope, and the Flask team for a lovely Python microframework!'
