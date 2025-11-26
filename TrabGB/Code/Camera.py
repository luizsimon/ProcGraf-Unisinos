import cv2
import numpy as np
import os 

script_dir = os.path.dirname(os.path.abspath(__file__))

parent_dir = os.path.dirname(script_dir)

image_folder = os.path.join(script_dir, 'images')

save_folder = os.path.join(parent_dir, 'savedimages')

# Garante que a pasta 'savedimages' exista
if not os.path.exists(save_folder):
    os.makedirs(save_folder)
    print(f"Pasta 'savedimages' criada em: {save_folder}")

# --- FUNÇÃO DE CARREGAMENTO (que evita bugs de caminho do OpenCV) ---
def load_sticker(file_name):
    full_path = os.path.join(image_folder, file_name)
    
    try:
        # Lê os bytes primeiro para evitar erros de caracteres no caminho
        with open(full_path, 'rb') as f:
            file_bytes = np.frombuffer(f.read(), dtype=np.uint8)
        
        # Decodifica os bytes em uma imagem OpenCV
        img = cv2.imdecode(file_bytes, cv2.IMREAD_UNCHANGED)
        
        if img is None:
            raise Exception("cv2.imdecode falhou (arquivo corrompido ou formato inválido?)")
            
        return img
        
    except Exception as e:
        print(f"ERRO CRÍTICO ao carregar {full_path}: {e}")
        return None

# Dicionário para mapear teclas aos filtros
filters = {
    ord('c'): {'active': False, 'function': lambda frame: cv2.Canny(frame, 50, 100)},
    ord('b'): {'active': False, 'function': lambda frame: cv2.GaussianBlur(frame, (15, 15), 0)},
    ord('m'): {'active': False, 'function': lambda frame: apply_magenta(frame)},
    ord('s'): {'active': False, 'function': lambda frame: cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)},
    ord('l'): {'active': False, 'function': lambda frame: cv2.Laplacian(frame, cv2.CV_64F)},
    ord('h'): {'active': False, 'function': lambda frame: cv2.equalizeHist(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))},
    ord('t'): {'active': False, 'function': lambda frame: cv2.threshold(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)[1]},
    ord('e'): {'active': False, 'function': lambda frame: cv2.erode(frame, np.ones((5, 5), np.uint8), iterations=1)},
    ord('d'): {'active': False, 'function': lambda frame: cv2.dilate(frame, np.ones((5, 5), np.uint8), iterations=1)},
    ord('n'): {'active': False, 'function': lambda frame: cv2.bitwise_not(frame)}
}

# --- 4. DICIONÁRIO DE STICKERS (Usando o caminho correto) ---
stickers_map = {
    ord('u'): load_sticker('unisinos.png'), 
    ord('a'): load_sticker('gato.png'),
    ord('g'): load_sticker('cabra.png'),
    ord('i'): load_sticker('cachorro.png'),
    ord('k'): load_sticker('emoji.png'),
}

# --- 5. VERIFICAÇÃO FINAL ---
print("\n--- Verificando carregamento dos stickers (Última tentativa) ---")
for key, img in stickers_map.items():
    if img is None:
        print(f"FALHA: Não foi possível carregar o sticker da tecla '{chr(key)}'")
    else:
        print(f"OK: Sticker '{chr(key)}' carregado com sucesso.")
print("-------------------------------------------\n")


# Variável global para armazenar o sticker selecionado
current_sticker = None

# Lista para armazenar as posições dos stickers e os stickers correspondentes
stickers = []

def apply_magenta(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    magenta = np.zeros_like(frame)
    magenta[:, :, 0] = gray * 0.5  # Canal Azul - Menor intensidade
    magenta[:, :, 1] = gray * 0    # Canal Verde - Zero intensidade
    magenta[:, :, 2] = gray        # Canal Vermelho - Maior intensidade
    return magenta

def display_instructions():
    print("\nINSTRUÇÕES DE COMANDO")
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

def overlay_sticker(background, sticker, x, y):
    # Dimensões do sticker
    sticker_height, sticker_width = sticker.shape[:2]
    
    # Verifica se o sticker tem 4 canais (RGBA) ou 3 canais (RGB)
    if sticker.shape[2] == 4:
        # Canal Alfa presente
        sticker_alpha = sticker[:, :, 3] / 255.0
        bg_alpha = 1.0 - sticker_alpha

        # Limites da área do sticker na imagem de fundo
        y1, y2 = y, y + sticker_height
        x1, x2 = x, x + sticker_width
        
        # Verifica se o sticker cabe na imagem de fundo sem ultrapassar os limites
        if y2 > background.shape[0] or x2 > background.shape[1]:
            return background
        
        # Calcula a região de interesse (ROI) para a aplicação do sticker
        roi = background[y1:y2, x1:x2]
        
        # Combinar o sticker com a imagem de fundo usando a máscara alfa
        for c in range(0, 3):
            background[y1:y2, x1:x2, c] = (sticker_alpha * sticker[:, :, c] +
                                            bg_alpha * background[y1:y2, x1:x2, c])
    else:
        # Sem canal Alfa, aplica o sticker diretamente
        y1, y2 = y, y + sticker_height
        x1, x2 = x, x + sticker_width
        
        # Verifica se o sticker cabe na imagem de fundo sem ultrapassar os limites
        if y2 > background.shape[0] or x2 > background.shape[1]:
            return background
        
        # Aplica o sticker diretamente na região de interesse (ROI)
        background[y1:y2, x1:x2] = sticker
    
    return background

# Função para capturar eventos do mouse
def mouse_callback(event, x, y, flags, param):
    global stickers, current_sticker
    
    if event == cv2.EVENT_LBUTTONDOWN and current_sticker is not None:
        stickers.append((x, y, current_sticker))  # Adiciona a posição do sticker e o sticker à lista

def edit_image(image_path):
    global current_sticker, stickers

    # Carrega a imagem
    image = cv2.imread(image_path)
    if image is None:
        print("Não foi possível carregar a imagem. Verifique o caminho e tente novamente.")
        return

    display_instructions()

    # Cria a janela onde a imagem editada será exibida
    cv2.namedWindow('Imagem - Editada')
    cv2.setMouseCallback('Imagem - Editada', mouse_callback)
    salvar = 0
    count = 0
    while True:
        frame_filtered = image.copy()
    
        key = cv2.waitKey(1) & 0xFF
        
        if key == ord('f'):
            break
        
        if key == ord('r'):
            for filter_key in filters:
                filters[filter_key]['active'] = False
        
        if key == ord('q'):
            salvar = 1

        if key in filters:
            filter_key = key
            for fkey in filters:
                if fkey != filter_key:
                    filters[fkey]['active'] = False
            filters[filter_key]['active'] = not filters[filter_key]['active']
        
        if key in stickers_map:
            current_sticker = stickers_map[key]
        
        for key_filter in filters:
            if filters[key_filter]['active']:
                frame_filtered = filters[key_filter]['function'](frame_filtered)
                if len(frame_filtered.shape) == 2:
                    frame_filtered = cv2.cvtColor(frame_filtered, cv2.COLOR_GRAY2BGR)
        
        for (x, y, sticker) in stickers:
            frame_filtered = overlay_sticker(frame_filtered, sticker, x, y)

        if salvar == 1:
            # Define o caminho completo para salvar
            save_path = os.path.join(save_folder, f'imagem_editada{count}.jpg')
            cv2.imwrite(save_path, frame_filtered)
            print(f"Imagem editada salva em: {save_path}")
            salvar = 0
            count+=1

        # Exibe o frame filtrado
        cv2.imshow('Imagem - Editada', frame_filtered)

    cv2.destroyAllWindows()
    stickers = []

def edit_webcam():
    global current_sticker, stickers # (Você tinha esquecido 'stickers' aqui)

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Não foi possível abrir a câmera.")
        return

    display_instructions()

    cv2.namedWindow('Webcam - Editada')
    cv2.setMouseCallback('Webcam - Editada', mouse_callback)
    salvar = 0
    count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Erro ao capturar o quadro.")
            break

        frame_filtered = frame.copy()
        
        key = cv2.waitKey(1)
        
        if key == ord('f'):
            break
        
        if key == ord('r'):
            for filter_key in filters:
                filters[filter_key]['active'] = False
 
        if key == ord('q'):
            salvar = 1
        
        if key in filters:
            filter_key = key
            for fkey in filters:
                if fkey != filter_key:
                    filters[fkey]['active'] = False
            filters[filter_key]['active'] = not filters[filter_key]['active']
        
        if key in stickers_map:
            current_sticker = stickers_map[key]
        
        for key_filter in filters:
            if filters[key_filter]['active']:
                frame_filtered = filters[key_filter]['function'](frame_filtered)
                if len(frame_filtered.shape) == 2:
                    frame_filtered = cv2.cvtColor(frame_filtered, cv2.COLOR_GRAY2BGR)

        for (x, y, sticker) in stickers:
            frame_filtered = overlay_sticker(frame_filtered, sticker, x, y)

        if salvar == 1: 
            # Define o caminho completo para salvar
            save_path = os.path.join(save_folder, f'webcam_editada{count}.jpg')
            cv2.imwrite(save_path, frame_filtered)
            print(f"Frame da webcam editado salvo em: {save_path}")
            salvar = 0
            count+=1

        cv2.imshow('Webcam - Editada', frame_filtered)

    cap.release()
    cv2.destroyAllWindows()
    stickers = [] # Limpa os stickers aqui

def main():
    global current_sticker, stickers

    while True:
        print("\nGustavo e Luiz - Processamento Gráfico")
        print("\n-------------------------------")
        print("1: Editar imagem")
        print("2: Editar vídeo da webcam")
        print("3: Sair")
        print("\n-------------------------------")
        choice = input("Selecione uma opção: ")

        if choice == '1':
            # Limpar estado de edição
            current_sticker = None
            stickers = []
            for filter_key in filters:
                filters[filter_key]['active'] = False

            image_path = input("\nDigite o caminho da imagem: ")
            edit_image(image_path)
        elif choice == '2':
            # Limpar estado de edição
            current_sticker = None
            stickers = []
            for filter_key in filters:
                filters[filter_key]['active'] = False

            edit_webcam()
        elif choice == '3':
            break
        else:
            print("\nOpção inválida. Tente novamente.")

if __name__ == "__main__":
    main()
