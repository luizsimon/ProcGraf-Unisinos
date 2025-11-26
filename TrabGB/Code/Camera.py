import cv2
import numpy as np
import os 

# ============================
#  PASTAS E CARREGAMENTO
# ============================

script_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(script_dir)
image_folder = os.path.join(script_dir, 'images')
save_folder = os.path.join(parent_dir, 'savedimages')

if not os.path.exists(save_folder):
    os.makedirs(save_folder)
    print(f"Pasta 'savedimages' criada em: {save_folder}")

def load_sticker(file_name):
    full_path = os.path.join(image_folder, file_name)
    try:
        with open(full_path, 'rb') as f:
            file_bytes = np.frombuffer(f.read(), dtype=np.uint8)
        img = cv2.imdecode(file_bytes, cv2.IMREAD_UNCHANGED)
        return img
    except Exception as e:
        print(f"ERRO ao carregar {file_name}: {e}")
        return None

# =============================
#  FUNÇÕES DE FILTROS
# =============================

def apply_magenta(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    magenta = np.zeros_like(frame)
    magenta[:,:,0] = gray * 0.5
    magenta[:,:,1] = 0
    magenta[:,:,2] = gray
    return magenta

def channel_blue(frame):
    blue = frame[:,:,0]
    return cv2.cvtColor(blue, cv2.COLOR_GRAY2BGR)

def channel_green(frame):
    green = frame[:,:,1]
    return cv2.cvtColor(green, cv2.COLOR_GRAY2BGR)

def channel_red(frame):
    red = frame[:,:,2]
    return cv2.cvtColor(red, cv2.COLOR_GRAY2BGR)

# ============================
#  LOAD DA IMAGEM PARA OPERAÇÕES MATEMÁTICAS
# ============================

math_image_path = os.path.join(image_folder, "math.png")
math_image = None

if os.path.exists(math_image_path):
    math_image = cv2.imread(math_image_path)
    print("Imagem para operações matemáticas carregada: math.png")
else:
    print("⚠️ math.png não encontrada em /images — operações matemáticas funcionarão só se você colocar uma imagem lá.")

# ============================
#  DICIONÁRIO DE FILTROS
# ============================

filters = {
    ord('c'): {'active': False, 'function': lambda f: cv2.Canny(f, 50, 100)},
    ord('b'): {'active': False, 'function': lambda f: cv2.GaussianBlur(f, (15,15), 0)},
    ord('m'): {'active': False, 'function': apply_magenta},
    ord('s'): {'active': False, 'function': lambda f: cv2.Sobel(f, cv2.CV_64F, 1,0, ksize=5)},
    ord('l'): {'active': False, 'function': lambda f: cv2.Laplacian(f, cv2.CV_64F)},
    ord('h'): {'active': False, 'function': lambda f: cv2.equalizeHist(cv2.cvtColor(f, cv2.COLOR_BGR2GRAY))},
    ord('t'): {'active': False, 'function': lambda f: cv2.threshold(cv2.cvtColor(f, cv2.COLOR_BGR2GRAY),127,255,cv2.THRESH_BINARY)[1]},
    ord('e'): {'active': False, 'function': lambda f: cv2.erode(f, np.ones((5,5),np.uint8),1)},
    ord('d'): {'active': False, 'function': lambda f: cv2.dilate(f, np.ones((5,5),np.uint8),1)},
    ord('n'): {'active': False, 'function': lambda f: cv2.bitwise_not(f)},

    # Seleção de canais
    ord('x'): {'active': False, 'function': channel_red},
    ord('y'): {'active': False, 'function': channel_green},
    ord('z'): {'active': False, 'function': channel_blue},

    # ============================
    #  OPERAÇÕES MATEMÁTICAS
    # ============================
    ord('1'): {'active': False, 'function': "add"},
    ord('2'): {'active': False, 'function': "subtract"},
    ord('3'): {'active': False, 'function': "blend"},
}

# =============================
#  STICKERS
# =============================

stickers_map = {
    ord('u'): load_sticker('unisinos.png'),
    ord('a'): load_sticker('gato.png'),
    ord('g'): load_sticker('cabra.png'),
    ord('i'): load_sticker('cachorro.png'),
    ord('k'): load_sticker('emoji.png'),
}

current_sticker = None
stickers = []

# =============================
#  DESCRIÇÕES DOS FILTROS
# =============================

filter_descriptions = {
    ord('c'): "Canny Edge Detection: detecta bordas (50-100).",
    ord('b'): "Gaussian Blur: kernel 15x15.",
    ord('m'): "Magenta (cinza + magenta).",
    ord('s'): "Sobel vertical.",
    ord('l'): "Laplacian.",
    ord('h'): "Equalização de histograma.",
    ord('t'): "Threshold 127 binário.",
    ord('e'): "Erode 5x5.",
    ord('d'): "Dilate 5x5.",
    ord('n'): "Negativo.",
    ord('x'): "Canal RED.",
    ord('y'): "Canal GREEN.",
    ord('z'): "Canal BLUE.",

    # Matemáticas
    ord('1'): "Adição entre imagem e math.png.",
    ord('2'): "Subtração entre imagem e math.png.",
    ord('3'): "Blending: 70% original + 30% math.png.",
}

# =============================
#  OPERAÇÕES MATEMÁTICAS
# =============================

def apply_math_operation(frame, op):
    if math_image is None:
        print("⚠️ math.png não encontrada — operação ignorada.")
        return frame

    resized = cv2.resize(math_image, (frame.shape[1], frame.shape[0]))

    if op == "add":
        return cv2.add(frame, resized)
    elif op == "subtract":
        return cv2.subtract(frame, resized)
    elif op == "blend":
        return cv2.addWeighted(frame, 0.7, resized, 0.3, 0)

    return frame

# =============================
#  FUNÇÕES DE INTERFACE
# =============================

def display_instructions():
    print("\nINSTRUÇÕES")
    print("-------------------------------")
    print("c: Canny edge detection")
    print("b: Gaussian blur")
    print("m: Magenta filter")
    print("s: Sobel filter")
    print("l: Laplacian filter")
    print("h: Histogram equalization")
    print("t: Binary thresholding")
    print("e: Erosion")
    print("d: Dilation")
    print("n: Negative transformation")
    print("-------------------------------")
    print("r: Remover o filtro atual")
    print("f: Encerrar edição")
    print("q: Salvar")
    print("-------------------------------")
    print("u: Adicionar sticker Unisinos")
    print("a: Adicionar sticker Gato")
    print("g: Adicionar sticker Cabra")
    print("i: Adicionar sticker Cachorro")
    print("k: Adicionar sticker Emoji")
    print("-------------------------------")
    print("x: Canal RED")
    print("y: Canal GREEN")
    print("z: Canal BLUE")
    print("-------------------------------")
    print("Operações matemáticas:")
    print("1 - Adição")
    print("2 - Subtração")
    print("3 - Mistura (Blending)")
    print("r: Remover filtros")
    print("q: Salvar")
    print("f: Finalizar")

def overlay_sticker(bg, sticker, x, y):
    h, w = sticker.shape[:2]
    if sticker.shape[2] == 4:
        alpha = sticker[:,:,3] / 255
        for c in range(3):
            bg[y:y+h, x:x+w, c] = alpha * sticker[:,:,c] + (1-alpha) * bg[y:y+h, x:x+w, c]
    else:
        bg[y:y+h, x:x+w] = sticker
    return bg

def mouse_callback(event,x,y,flags,param):
    global stickers, current_sticker
    if event == cv2.EVENT_LBUTTONDOWN and current_sticker is not None:
        stickers.append((x,y,current_sticker))

# =============================
#  APLICAR FILTROS E OPERAÇÕES
# =============================

def apply_filters(frame):
    for key, data in filters.items():
        if data['active']:
            if data['function'] in ["add", "subtract", "blend"]:
                frame = apply_math_operation(frame, data['function'])
            else:
                frame = data['function'](frame)
                if len(frame.shape) == 2:
                    frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)
    return frame

# =============================
#  EDITAR IMAGEM
# =============================

def edit_image(path):
    global current_sticker, stickers

    image = cv2.imread(path)
    if image is None:
        print("Erro ao carregar imagem")
        return

    display_instructions()
    cv2.namedWindow("Editada")
    cv2.setMouseCallback("Editada", mouse_callback)

    salvar = False
    count = 0

    while True:
        frame = image.copy()
        key = cv2.waitKey(1) & 0xFF

        if key == ord('f'): break
        if key == ord('r'):
            for k in filters: filters[k]['active'] = False
        if key == ord('q'): salvar = True
        if key in filters:
            for k in filters:
                filters[k]['active'] = False
            filters[key]['active'] = True
            if key in filter_descriptions:
                print("\n[FILTRO ATIVADO] " + filter_descriptions[key])

        if key in stickers_map:
            current_sticker = stickers_map[key]

        frame = apply_filters(frame)

        for (x,y,stk) in stickers:
            frame = overlay_sticker(frame, stk, x,y)

        if salvar:
            save_path = os.path.join(save_folder, f"editada_{count}.jpg")
            cv2.imwrite(save_path, frame)
            print("Salvo em:", save_path)
            salvar = False
            count += 1

        cv2.imshow("Editada", frame)

    cv2.destroyAllWindows()
    stickers = []

# =============================
#  EDITAR WEBCAM
# =============================

def edit_webcam():
    global current_sticker, stickers

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Erro ao abrir webcam")
        return

    display_instructions()
    cv2.namedWindow("Webcam")
    cv2.setMouseCallback("Webcam", mouse_callback)

    salvar = False
    count = 0

    while True:
        ret, frame = cap.read()
        if not ret: break

        key = cv2.waitKey(1) & 0xFF

        if key == ord('f'): break
        if key == ord('r'):
            for k in filters: filters[k]['active'] = False
        if key == ord('q'): salvar = True
        if key in filters:
            for k in filters:
                filters[k]['active'] = False
            filters[key]['active'] = True
            if key in filter_descriptions:
                print("\n[FILTRO ATIVADO] " + filter_descriptions[key])
        if key in stickers_map:
            current_sticker = stickers_map[key]

        frame = apply_filters(frame)

        for (x,y,stk) in stickers:
            frame = overlay_sticker(frame, stk, x,y)

        if salvar:
            save_path = os.path.join(save_folder, f"webcam_{count}.jpg")
            cv2.imwrite(save_path, frame)
            print("Salvo em:", save_path)
            salvar = False
            count += 1

        cv2.imshow("Webcam", frame)

    cap.release()
    cv2.destroyAllWindows()
    stickers = []

# =============================
#  MAIN
# =============================

def main():
    global current_sticker, stickers

    while True:
        print("\n1 - Editar imagem")
        print("2 - Webcam")
        print("3 - Sair")
        opt = input("Opção: ")

        for k in filters:
            filters[k]['active'] = False
        stickers = []
        current_sticker = None

        if opt == "1":
            p = input("Caminho da imagem: ")
            edit_image(p)
        elif opt == "2":
            edit_webcam()
        elif opt == "3":
            break

if __name__ == "__main__":
    main()
