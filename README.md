# UN-Robotics

En este repositorio se desarrolla una libreria de software para control de robots educativos, con foco en plataformas de recursos limitados (ESP32 Dev Board CH340 38PIN USB C). El objetivo es ofrecer una herramienta robusta y eficiente que permita a estudiantes de secundaria programar sus propios robots sin preocuparse por detalles complejos de software.

## Organización del repositorio

El repositorio se organizará de la siguiente manera:

```txt
UN-Robotics/
├── TARS/
│   ├── TARS.h
│   ├── Sensors/
│   │   ├── Sensor.h
│   │   ├── Ultrasonic.h
│   │   └── ...
│   ├── Actuators/
│   │   ├── Actuator.h
│   │   ├── Motor.h
│   │   └── ...
│   └── ...
├── .gitignore
├── License
├── README.md
└── ...
```

La razón de organizar el código de esta manera es para mantener una estructura en la que importando TARS.h, se importen automáticamente todos los sensores y actuadores disponibles, facilitando así su uso por parte de los estudiantes. En cambio, si se importa solo TARS/Sensors/Sensor.h, solo se importará la clase Sensor, lo que permite probar componentes específicos sin necesidad de cargar toda la librería. Esta organización modular también facilita la escalabilidad y el mantenimiento del código a medida que se agreguen nuevas funcionalidades en el futuro.

## Pautas de desarrollo

Con tal de mantener calidad técnica y asegurar una buena experiencia por parte del estudiante que vaya a usar la librería, se deberán seguir las siguientes pautas, aparte de las que ya se encuentran en el drive del equipo. 

Para la parte externa de la librería:

- **Simplicidad**: La interfaz de la librería debe ser sencilla y fácil de entender para estudiantes de secundaria. Se deben evitar conceptos avanzados de programación y enfocarse en funciones básicas que permitan controlar los sensores y actuadores de manera intuitiva.

- **Documentación clara**: Se debe proporcionar una documentación clara y concisa que explique cómo usar la librería, con ejemplos prácticos y tutoriales paso a paso. Esto ayudará a los estudiantes a comprender rápidamente cómo implementar sus proyectos de robótica. Además, se deben incluir comentarios en el código para explicar la funcionalidad de cada función y cómo se relaciona con los componentes del robot.

Con respecto a la parte interna de la librería, se deben seguir las siguientes pautas para asegurar un código limpio, eficiente y mantenible:

- **Modularidad**: El código deberá estar organizado en módulos o clases que representen los diferentes componentes que se pueden controlar con la librería (motores, sensores, etc.). Esto facilitará la escalabilidad y el mantenimiento del código a medida que se agreguen nuevas funcionalidades.

- **Eficiencia**: Dado que se está trabajando con plataformas de recursos limitados, es crucial optimizar el código para asegurar un rendimiento adecuado. Se deben evitar operaciones innecesarias y utilizar estructuras de datos eficientes para manejar la información. Esto quiere decir, por ejemplo, evitar el uso de Strings dinámicos y preferir el uso de arrays de caracteres para manejar texto, o utilizar uint8_t en lugar de int para variables que solo necesitan almacenar valores pequeños positivos.

- **Mantenibilidad**: El código debe ser fácil de entender y modificar en el futuro. Se deben seguir buenas prácticas de programación, como nombrar variables y funciones de manera descriptiva, evitar la duplicación de código y mantener una estructura clara y coherente.

- **Comentarios y documentación interna**: Además de la documentación externa, es importante incluir comentarios dentro del código para explicar la lógica detrás de las funciones y cualquier decisión de diseño importante. Esto facilitará que otros desarrolladores puedan entender y contribuir al código en el futuro. Los comentarios deberán seguir el estandard de documentación Doxygen para C++ (o el standard correspondiente para el lenguaje utilizado).

En resumen, el desarrollo de esta librería de software para control de robots educativos debe enfocarse en ofrecer una experiencia de usuario sencilla y accesible para estudiantes de secundaria, mientras se mantiene un código interno limpio, eficiente y fácil de mantener. Esto permitirá que los estudiantes puedan aprender y experimentar con la robótica de manera efectiva, sin preocuparse por detalles técnicos complejos.
