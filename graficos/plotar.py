import re
import time
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# 1. Função Schaffer's f6 para as curvas de nível
def schaffer_f6(x, y):
    num = (np.sin(np.sqrt(x**2 + y**2)))**2 - 0.5
    den = (1.0 + 0.001 * (x**2 + y**2))**2
    return 0.5 - (num / den)

# Leitura dos dados
print("Lendo o arquivo de saída...")
with open('saida.txt', 'r') as f:
    text = f.read()

# Parsing dos dados
geracoes_text = text.split('<------Geração ')[1:]
historico_populacao = []
fit_melhor, fit_pior, fit_media = [], [], []
melhor_xy = (0, 0)

for gen in geracoes_text:
    pontos_str = re.findall(r'\(([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?),\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\)', gen)
    pop_gen = [(float(x), float(y)) for x, y in pontos_str[:-2]] 
    historico_populacao.append(pop_gen)
    
    media = float(re.search(r'Média:\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)', gen).group(1))
    melhor = float(re.search(r'Melhor \(([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\):', gen).group(1))
    pior = float(re.search(r'Pior \(([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\):', gen).group(1))
    
    fit_media.append(media)
    fit_melhor.append(melhor)
    fit_pior.append(pior)
    melhor_xy = (float(pontos_str[-2][0]), float(pontos_str[-2][1]))

print(f"Coordenadas do melhor indivíduo ao final: X={melhor_xy[0]:.6f}, Y={melhor_xy[1]:.6f}")
print(f"Fitness do melhor indivíduo ao final: {fit_melhor[-1]}")

# Gerando e salvando os gráficos sem travar a execução
plt.figure(figsize=(10, 5))
plt.plot(fit_melhor, label="Melhor Fitness", color='green')
plt.plot(fit_pior, label="Pior Fitness", color='red')
plt.plot(fit_media, label="Média da População", color='blue', linestyle='--')
plt.title("Evolução do Fitness (Todos)")
plt.xlabel("Gerações")
plt.ylabel("Fitness")
plt.legend()
plt.grid(True)
plt.savefig("fitness_todos.png")
plt.close()

plt.figure(figsize=(10, 5))
plt.plot(fit_melhor, label="Melhor Fitness", color='green', linewidth=2)
plt.title("Evolução do Melhor Indivíduo")
plt.xlabel("Gerações")
plt.ylabel("Fitness")
plt.legend()
plt.grid(True)
plt.savefig("fitness_melhor.png")
plt.close()

# -------------------------------------------------------------
# GERANDO O GIF COM AVISO, PROGRESSO E TEMPO
# -------------------------------------------------------------
# -------------------------------------------------------------
# GERANDO O GIF COM AVISO, PROGRESSO E TEMPO
# -------------------------------------------------------------
print("\n[AVISO] Iniciando o processamento do GIF animado...")
inicio_tempo = time.time()

fig, ax = plt.subplots(figsize=(8, 8))
ax.set_xlim(-10, 10)
ax.set_ylim(-10, 10)

X = np.linspace(-10, 10, 400)
Y = np.linspace(-10, 10, 400)
X, Y = np.meshgrid(X, Y)
Z = schaffer_f6(X, Y)
contour = ax.contourf(X, Y, Z, levels=20, cmap='viridis', alpha=0.8)
fig.colorbar(contour, ax=ax, shrink=0.8)

scatter = ax.scatter([], [], c='red', edgecolors='black', s=50)
total_frames = len(historico_populacao)

def init():
    scatter.set_offsets(np.empty((0, 2)))
    return scatter,

def update(frame):
    pop_atual = historico_populacao[frame]
    scatter.set_offsets(pop_atual)
    ax.set_title(f"Geração {frame}")
    
    # Imprime o progresso diretamente na renderização de cada quadro
    porcentagem = ((frame + 1) / total_frames) * 100
    print(f"\rProcessando GIF: {porcentagem:.1f}% ({frame + 1}/{total_frames} quadros)", end="", flush=True)
    
    return scatter,

ani = FuncAnimation(fig, update, frames=total_frames, init_func=init, blit=True)

# Salva o GIF sem o progress_callback problemático
ani.save('evolucao_populacao.gif', writer='pillow', fps=10)
plt.close()

tempo_decorrido = time.time() - inicio_tempo
print(f"\n[SUCESSO] Processamento do GIF concluído em {tempo_decorrido:.2f} segundos!")