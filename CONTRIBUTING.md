# Contribucion a UN-Robotics

Este documento recoge pautas para el equipo tecnico que mantiene la libreria.

## Objetivo

Mantener una API simple para alumnado/profesorado y una base de codigo modular, eficiente y mantenible.

## Pautas para la API publica

- Simplicidad: priorizar metodos directos y faciles de entender para estudiantes de secundaria.
- Claridad: documentar cada clase y metodo con ejemplos cortos y practicos.
- Estabilidad: evitar cambios rompientes en nombres de clases/metodos usados en ejemplos.

## Pautas internas de implementacion

- Modularidad: separar sensores, actuadores y comunicaciones en clases independientes.
- Eficiencia: evitar operaciones costosas en ESP32 cuando no aporten valor.
- Mantenibilidad: nombres descriptivos, poco acoplamiento y evitar duplicacion.
- Documentacion interna: usar comentarios de tipo Doxygen cuando aplique.

## Flujo recomendado

1. Añadir o actualizar ejemplo en `examples/`.
2. Verificar compilacion en Arduino IDE para ESP32.
3. Mantener `README.md` orientado a uso en aula.
4. Mantener este archivo para criterios tecnicos del equipo.
