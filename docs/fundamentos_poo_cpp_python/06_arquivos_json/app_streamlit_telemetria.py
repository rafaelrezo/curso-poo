from pathlib import Path
import json

import pandas as pd
import streamlit as st


CAMINHO_PADRAO = Path("telemetria.json")


def carregar_json():
    arquivo_enviado = st.file_uploader("Arquivo JSON", type=["json"])

    if arquivo_enviado is not None:
        return json.load(arquivo_enviado)

    if CAMINHO_PADRAO.exists():
        return json.loads(CAMINHO_PADRAO.read_text(encoding="utf-8"))

    st.warning("telemetria.json nao encontrado.")
    st.stop()


st.set_page_config(page_title="Telemetria JSON", layout="wide")
st.title("Telemetria JSON")

dados = carregar_json()
leituras = dados.get("leituras", [])

if not leituras:
    st.error("O JSON nao possui leituras.")
    st.stop()

df = pd.DataFrame(leituras)
df["timestamp"] = pd.to_datetime(df["timestamp"], utc=True, errors="coerce")
df = df.dropna(subset=["timestamp"]).sort_values("timestamp")

col_cpu, col_memoria, col_temp = st.columns(3)
col_cpu.metric("CPU media", f"{df['cpu_percent'].mean():.1f}%")
col_memoria.metric("Memoria media", f"{df['memoria_percent'].mean():.1f}%")
col_temp.metric("Temperatura media", f"{df['temperatura_c'].mean():.1f} C")

serie = st.selectbox(
    "Serie",
    ["cpu_percent", "memoria_percent", "temperatura_c"],
)

st.line_chart(df.set_index("timestamp")[[serie]])
st.dataframe(df, use_container_width=True)
