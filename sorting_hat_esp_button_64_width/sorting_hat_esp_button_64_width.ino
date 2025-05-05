#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sorting_hat_model.h"  // ML Model

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Pins
#define BUTTON_A  13  // Button for Answer A
#define BUTTON_B  12  // Button for Answer B
#define BUTTON_C  32  // Button for Answer C
#define BUTTON_D  25  // Button for Answer D

// Sorting Hat Questions
const char* questions[] = {
    "1. Risk or safe?",
    "2. How decide?",
    "3. Most valued trait?",
    "4. Your group role?",
    "5. Face challenge how?",
    "6. Teamwork style?",
    "7. How you learn?",
    "8. Rules are...?",
    "9. Strength?",
    "10. Best kind of friend?"
};

// Answer Options
const char* options[][4] = {
    {"A) Risk-taker", "B) Strategic", "C) Supportive", "D) Cautious"},
    {"A) Instinct", "B) Ambition", "C) Loyalty", "D) Logic"},
    {"A) Courage", "B) Determination", "C) Patience", "D) Intelligence"},
    {"A) Leader", "B) Initiator", "C) Helper", "D) Analyst"},
    {"A) Face it head-on", "B) Plan and attack", "C) Work together", "D) Think it through"},
    {"A) Bold and direct", "B) Persuasive and driven", "C) Caring and inclusive", "D) Observant and precise"},
    {"A) Hands-on", "B) Ambitious projects", "C) Group discussions", "D) Reading and thinking alone"},
    {"A) Can be challenged", "B) Tools for winning", "C) Should be respected", "D) Can be improved"},
    {"A) Bravery", "B) Drive", "C) Kindness", "D) Wisdom"},
    {"A) Loyal in danger", "B) Pushes you forward", "C) Always there for you", "D) Thinks deeply & understands you"}
};

// Store user responses (1-4)
int responses[10] = {0};
int questionIndex = 0;  // Current question index

// Initialize Machine Learning Model
Eloquent::ML::Port::DecisionTree clf;

// Button debounce variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
    Serial.begin(115200);

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }
    display.clearDisplay();

    // Welcome Message
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("Sorting Hat Ready!");
    display.display();
    delay(2000);

    // Initialize buttons as input
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    pinMode(BUTTON_D, INPUT_PULLUP);

    showQuestion();
}

void loop() {
    checkButtons();  // Check for user input
}

// Function to display questions on OLED
void showQuestion() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(questions[questionIndex]);
    
    // Display answer choices
    for (int i = 0; i < 4; i++) {
        display.setCursor(10, 20 + (i * 10));
        display.println(options[questionIndex][i]);
    }

    display.display();
}

// Function to check button presses and record response
// Each button corresponds to one of the four options (1–4)
// Once a valid button is pressed, we store the value and move to the next question
void checkButtons() {
    // Check each button with improved debounce logic
    if (digitalRead(BUTTON_A) == LOW) {                     
        if (millis() - lastDebounceTime > debounceDelay) {  
            responses[questionIndex] = 1;                   // Record response as option A
            Serial.println("Button A pressed");
            lastDebounceTime = millis();
            delay(300);                                     // Debounce delay to avoid double triggers
            nextQuestion();                                 // Proceed to next question
        }
        while(digitalRead(BUTTON_A) == LOW); // Wait for button release
        return;
    }
    
    if (digitalRead(BUTTON_B) == LOW) {
        if (millis() - lastDebounceTime > debounceDelay) {
            responses[questionIndex] = 2;                   // Record response as option B
            Serial.println("Button B pressed");
            lastDebounceTime = millis();
            delay(300); 
            nextQuestion();
        }
        while(digitalRead(BUTTON_B) == LOW);
        return;
    }
    
    if (digitalRead(BUTTON_C) == LOW) {
        if (millis() - lastDebounceTime > debounceDelay) {
            responses[questionIndex] = 3;                   // Record response as option C
            Serial.println("Button C pressed");
            lastDebounceTime = millis();
            delay(300); 
            nextQuestion();
        }
        while(digitalRead(BUTTON_C) == LOW);
        return;
    }
    
    if (digitalRead(BUTTON_D) == LOW) {
        if (millis() - lastDebounceTime > debounceDelay) {
            responses[questionIndex] = 4;                   // Record response as option D
            Serial.println("Button D pressed");
            lastDebounceTime = millis();
            delay(300); 
            nextQuestion();
        }
        while(digitalRead(BUTTON_D) == LOW);
        return;
    }
}

// Move to the next question or classify the house
void nextQuestion() {
    questionIndex++;
    if (questionIndex < 10) {
        showQuestion();  // Show next question
    } else {
        classifyHouse();  // Run ML model prediction
    }
}

// Function to classify the user into a Hogwarts house
void classifyHouse() {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Sorting...");

    // Convert responses into ML input format
    float features[] = { 
        (float)responses[0], (float)responses[1], (float)responses[2], (float)responses[3], 
        (float)responses[4], (float)responses[5], (float)responses[6], (float)responses[7], 
        (float)responses[8], (float)responses[9] 
    };

    int house = clf.predict(features);  // Use ML model

    // Display house result
    display.setCursor(10, 30);
    display.print("House: ");
    switch (house) {
        case 0: display.println("Gryffindor"); break;
        case 1: display.println("Hufflepuff"); break;
        case 2: display.println("Ravenclaw"); break;
        case 3: display.println("Slytherin"); break;
    }

    display.display();
    Serial.println("Sorting complete!");
    Serial.print("Predicted House: ");
    switch (house) {
        case 0: Serial.println("Gryffindor"); break;
        case 1: Serial.println("Hufflepuff"); break;
        case 2: Serial.println("Ravenclaw"); break;
        case 3: Serial.println("Slytherin"); break;
    }
}