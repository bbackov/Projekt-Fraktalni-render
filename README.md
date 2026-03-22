#  Fractal Renderer (OpenGL)

Jednostavan renderer fraktala izrađen u **C++**, koristeći **OpenGL**, **GLFW**, **GLAD** i **Dear ImGui** za prikaz parametara.

---

## Funkcionalnosti

- Renderiranje više tipova fraktala:
  - Mandelbrot
  - Julia
  - Burning Ship
  - Multibrot

- Interaktivna kontrola:
  - Zoom (scroll)
  - Pomicanje (drag mišem)
  - Promjena parametara u realnom vremenu

- Overlay UI (ImGui):
  - prikaz trenutnih vrijednosti (iteracije, zoom, parametri)
  - prikaz kontrola

---

## Kontrole

| Tipka / Akcija | Funkcija |
|---------------|--------|
| `1-4` | Odabir fraktala |
| `+ / -` | Povećanje/smanjenje iteracija |
| `G / F` | Promjena parametara (Julia / Multibrot) |
| `R` | Reset |
| Scroll | Zoom |
| LMB drag | Pomicanje |
| RMB | Promjena Julia konstante |

---

## Tehnologije

- C++
- OpenGL 3.3
- GLFW
- GLAD
- Dear ImGui

---

## Pokretanje (MSYS2 MinGW64)

```bash
g++ src/main.cpp src/glad.c src/shader.cpp \
external/imgui/imgui.cpp \
external/imgui/imgui_draw.cpp \
external/imgui/imgui_tables.cpp \
external/imgui/imgui_widgets.cpp \
external/imgui/backends/imgui_impl_glfw.cpp \
external/imgui/backends/imgui_impl_opengl3.cpp \
-Iinclude -Iexternal/imgui -Iexternal/imgui/backends \
-Llibs -lglfw3 -lopengl32 -lgdi32 \
-o fraktal.exe
