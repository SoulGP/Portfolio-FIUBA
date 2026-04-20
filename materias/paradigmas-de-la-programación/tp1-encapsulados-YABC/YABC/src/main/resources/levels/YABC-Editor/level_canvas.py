import tkinter as tk

CELL_SIZE = 20
GRID_WIDTH = 40
GRID_HEIGHT = 30
CANVAS_WIDTH = CELL_SIZE * GRID_WIDTH
CANVAS_HEIGHT = CELL_SIZE * GRID_HEIGHT

COLORS = {
    "player1": "#90ee90",       # Light Green
    "player2": "#006400",       # Dark Green
    "enemy_regular": "#8b4513", # Brown
    "enemy_heavy": "#ffd700",   # Yellow
    "enemy_fast": "#104e8b",    # Dark Blue
    "enemy_powerful": "#ff0000",# Red
    "base": "#ffffff",          # White
    "brick": "#a9a9a9",         # Gray
    "steel": "#555555",         # Dark Gray
    "water": "#87ceeb",         # Light Blue
    "forest": "#228b22",        # Forest Green
    "empty": "#000000"          # Black
}

class LevelCanvas:
    def __init__(self, parent, on_change_callback):
        self.canvas = tk.Canvas(parent, width=CANVAS_WIDTH, height=CANVAS_HEIGHT, bg="black")
        self.canvas.pack(side=tk.RIGHT)
        self.canvas.bind("<Button-1>", self.place_element)
        self.canvas.bind("<B1-Motion>", self.place_element)  # Pintado por arrastre

        self.selected_element = "brick"
        self.on_change_callback = on_change_callback

        self.grid = [["empty" for _ in range(GRID_WIDTH)] for _ in range(GRID_HEIGHT)]
        self.cell_ids = [[None for _ in range(GRID_WIDTH)] for _ in range(GRID_HEIGHT)]
        self.player_positions = {"player1": None, "player2": None}  # (py, px)
        self.base_position = None  # (py, px)
        self.COLORS = COLORS

        self.draw_grid()

    def draw_grid(self):
        for y in range(GRID_HEIGHT):
            for x in range(GRID_WIDTH):
                x1 = x * CELL_SIZE
                y1 = y * CELL_SIZE
                x2 = x1 + CELL_SIZE
                y2 = y1 + CELL_SIZE
                rect = self.canvas.create_rectangle(x1, y1, x2, y2,
                                                    outline="#333", fill=COLORS["empty"])
                self.cell_ids[y][x] = rect

    def set_selected_element(self, element_key):
        self.selected_element = element_key

    def place_element(self, event):
        x = event.x // CELL_SIZE
        y = event.y // CELL_SIZE
        if not (0 <= x < GRID_WIDTH and 0 <= y < GRID_HEIGHT):
            return

        px = x * CELL_SIZE + CELL_SIZE // 2
        py = y * CELL_SIZE + CELL_SIZE // 2

        # Evitar duplicación innecesaria
        current_tile = self.grid[y][x]
        if current_tile == self.selected_element:
            return

        # Remover posición anterior si es jugador o base
        if self.selected_element in ["player1", "player2"]:
            prev = self.player_positions[self.selected_element]
            if prev:
                old_y = (prev[0] - CELL_SIZE // 2) // CELL_SIZE
                old_x = (prev[1] - CELL_SIZE // 2) // CELL_SIZE
                self.grid[old_y][old_x] = "empty"
                self.canvas.itemconfig(self.cell_ids[old_y][old_x], fill=COLORS["empty"])
            self.player_positions[self.selected_element] = (py, px)

        elif self.selected_element == "base":
            if self.base_position:
                old_y = (self.base_position[0] - CELL_SIZE // 2) // CELL_SIZE
                old_x = (self.base_position[1] - CELL_SIZE // 2) // CELL_SIZE
                self.grid[old_y][old_x] = "empty"
                self.canvas.itemconfig(self.cell_ids[old_y][old_x], fill=COLORS["empty"])
            self.base_position = (py, px)

        self.grid[y][x] = self.selected_element
        self.canvas.itemconfig(self.cell_ids[y][x], fill=COLORS[self.selected_element])
        self.on_change_callback()

    def clear_grid(self):
        for y in range(GRID_HEIGHT):
            for x in range(GRID_WIDTH):
                self.grid[y][x] = "empty"
                self.canvas.itemconfig(self.cell_ids[y][x], fill=COLORS["empty"])
        self.player_positions = {"player1": None, "player2": None}
        self.base_position = None
        self.on_change_callback()

    def get_state(self):
        return {
            "grid": self.grid,
            "player_positions": self.player_positions,  # (py, px)
            "base_position": self.base_position         # (py, px)
        }