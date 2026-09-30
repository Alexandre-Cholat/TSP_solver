from .base_parser import BaseParser

# activate uv with: source .venv/bin/activate
# download frequirements: uv pip install -r requirements.txt
# add requirements : uv pip freeze > requirements.txt

class LowerTriParser(BaseParser):
    def parse(self):
        matrix_data = []
        reading_matrix = False

        for line in self.read_lines():
            if line.startswith("DIMENSION"):
                self.dimension = int(line.split(":")[-1].strip())
            elif line.startswith("EDGE_WEIGHT_SECTION"):
                reading_matrix = True
            elif reading_matrix:
                if line == "EOF":
                    break
                matrix_data.extend([float(val) for val in line.split()])

        return {"dimension": self.dimension, "weights": matrix_data}