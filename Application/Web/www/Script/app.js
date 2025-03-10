// Function to display messages in the chat
function displayMessage(sender, message) {
    const messagesDiv = document.getElementById("messages");
    const newMessage = document.createElement("div");
    newMessage.classList.add(sender === 'You' ? 'user-message' : 'phi-message');
    newMessage.innerHTML = `<strong>${sender}:</strong> ${message}`;
    messagesDiv.appendChild(newMessage);
    messagesDiv.scrollTop = messagesDiv.scrollHeight;  // Scroll to the latest message
}

// Start voice recognition
function startListening() {
    const recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
    recognition.lang = "en-US";
    recognition.start();

    recognition.onresult = function(event) {
        const speechToText = event.results[0][0].transcript;
        document.getElementById("userInput").value = speechToText;
        sendVoiceMessage(speechToText);
    };

    recognition.onerror = function(event) {
        //alert("Error in speech recognition: " + event.error);
        startListening();
    };
}

// Function to send the message to the backend
function sendVoiceMessage(message) {
    fetch('/cgi-bin/Application', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ Command: 'QUESTION', Value: message })
    })
    .then(response => response.json())
    .then(data => {
        displayMessage("You", message);
        displayMessage("mLam", data.Value);
        const utterance = new SpeechSynthesisUtterance(data.Value);
        window.speechSynthesis.speak(utterance);
        startListening();
    })
    .catch(error => {
        startListening();
    });
}

// Function to send typed message
function sendTextMessage() {
    const message = document.getElementById("userInput").value;
    sendVoiceMessage(message);
}
