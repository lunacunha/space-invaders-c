# Space Invaders

A classic game reimplemented for embedded systems, developed in C as the final project for the Computer Laboratory course (FEUP).

![Version](https://img.shields.io/badge/version-1.0-blue)
![Language](https://img.shields.io/badge/language-C-orange)
![Platform](https://img.shields.io/badge/platform-embedded-green)

## About

Complete implementation of the classic Space Invaders for microcontrollers, featuring real-time graphics rendering, persistent scoring system, and multiple advanced functionalities. The game uses low-level programming with direct hardware access, including keyboard controllers, timer, and VBE graphics interface.

## Features

### Gameplay
- Smooth ship control with **A/D** keys
- Shooting system with **SPACE**
- Enemies organized in 3×6 formation with coordinated movement
- Destructible shields with multiple damage states
- Precise real-time collision detection
- Dynamic scoring system with penalties
- Progressive difficulty

### Interface
- VBE graphics rendering with XPM sprites
- Double buffering for smooth 60 FPS animations
- Multi-state menus with intuitive navigation
- Real-time statistics during gameplay
- Persistent high score leaderboard

### Technical Features
- State machine-based architecture
- Precise timing with i8254 timer (60Hz)
- Interrupt-based i8042 keyboard driver
- Score storage in CSV file
- Random anime character name assignment to high scores

## Architecture

The project follows a modular structure with clear separation of concerns:
```
space-invaders/
├── controllers/
│   ├── graphics/      # VBE graphics interface and XPM sprites
│   ├── keyboard/      # i8042 driver and scancode interpretation
│   ├── mouse/         # PS/2 protocol (not implemented)
│   └── timer/         # i8254 timer management
├── game/
│   ├── enemies/       # Enemy logic and formation
│   ├── menu/          # Navigation system and states
│   ├── scores/        # Persistent scoring system
│   ├── ship/          # Player ship and shields
│   └── game_state/    # Main loop and state management
└── xpm/
    ├── characters/    # Fonts for custom text
    ├── game/          # Game sprites (ship, aliens, shields)
    └── menu/          # Menu visual elements
```

## Main Components

### Controllers
- **Graphics**: Graphics interface abstraction with VBE mode support, sprite rendering, and double buffering
- **Keyboard**: i8042 controller driver with scancode interpretation
- **Timer**: i8254 timer management for update rate control (60Hz)

### Game Logic
- **Enemies**: Enemy formation system with coordinated movement and automatic descent
- **Menu**: State machine-based navigation
- **Scores**: Score calculation with penalties and persistent storage
- **Ship**: Ship logic, shooting, and destructible shields
- **Game State**: Main game loop with complete state management

## Technologies

- **Language**: C
- **Hardware**: i8042 (keyboard), i8254 (timer)
- **Graphics**: VESA BIOS Extensions (VBE)
- **Sprite Format**: XPM (X PixMap)
- **Architecture**: Interrupt-based with state machine

## Controls

| Key | Action |
|-----|--------|
| **A** | Move left |
| **D** | Move right |
| **SPACE** | Shoot |

## Scoring System

The scoring system is dynamically calculated based on:
- Number of enemies eliminated
- Total game time (penalty)
- Wasted ammunition (penalty)

High scores are stored in a CSV file and associated with randomly assigned anime character names.

## Distinctive Features

Our project integrates multiple advanced components that leverage direct hardware access to deliver a smooth gaming experience:

- **Graphics Engine**: VBE with support for different color depths, XPM sprite rendering, and custom double buffering implementation ensuring smooth 60 FPS animations
- **Interrupt-Based Architecture**: Optimized drivers for keyboard, mouse, and timer allowing real-time response, parity error detection, and safe hardware state restoration
- **Efficient Memory Management**: Dynamic buffers, automatic color depth adaptation, and graphics resource reuse
- **Advanced Game Logic**: Enemies with synchronized formation movement, destructible shields with multiple states, and precise collision detection based on real bounding boxes
- **Dynamic Difficulty**: Game difficulty evolves as enemies are eliminated
- **State Machine**: Controls menus, gameplay, game over, and leaderboard screens

---

LCOM Project for group GRUPO_2LEIC08_3.

Group members:

1. Eduardo Cunha (up202207126@up.pt)
2. Júlio Santos (up202207975@up.pt)
3. Luna Cunha (up202205714@up.pt)
4. Rodrigo Araújo (up202205515@up.pt)


**FEUP** | Computer Laboratory | 2024/2025
