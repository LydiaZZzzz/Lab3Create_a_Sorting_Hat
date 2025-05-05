# Lab 3 – Sorting Hat (ESP32 + Machine Learning)



<video src="demo.mp4" controls width="600"></video>




## Reflections and System Improvements

After working with the sorting hat system, I realized that increasing the dataset size would significantly improve model accuracy. In addition to collecting real responses, we could also include “ideal” response-label pairs based on canonical character traits to enrich the training data.

During testing, I noticed that adding a short delay after each button press is critical—without it, the device often skipped questions due to button debounce issues. This small adjustment greatly improved the user experience.

## Interaction Enhancement Ideas

To create a more immersive and magical experience, I would consider replacing physical buttons with gesture-based input. For example, using a hand motion sensor or infrared proximity sensor embedded in a wand could allow users to "cast" their answers.

### Possible Sensor Options

- **APDS9960**: For gesture and proximity sensing  
- **VL53L0X**: For precise time-of-flight distance measurement  

## Model Adaptation Considerations

The decision tree model works well with discrete, button-style input. However, for gesture-based input—which can be continuous, noisy, or multidimensional—a decision tree may be too simplistic and fail to generalize well.

In this case, I would consider using a lightweight TinyML model, which is better suited to handle spatial or temporal patterns in sensor data.
