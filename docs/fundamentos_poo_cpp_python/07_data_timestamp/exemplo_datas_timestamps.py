from datetime import datetime, timedelta, timezone
from time import perf_counter, sleep


def iso_utc(instante: datetime) -> str:
    return instante.astimezone(timezone.utc).isoformat().replace("+00:00", "Z")


agora = datetime.now(timezone.utc)
timestamp_segundos = agora.timestamp()
proxima_leitura = agora + timedelta(minutes=15)

print(f"Agora em UTC: {iso_utc(agora)}")
print(f"Timestamp Unix em segundos: {timestamp_segundos:.0f}")
print(f"Proxima leitura: {iso_utc(proxima_leitura)}")

texto_json = "2026-04-29T12:00:00Z"
instante_parseado = datetime.fromisoformat(texto_json.replace("Z", "+00:00"))
print(f"Timestamp lido do JSON: {iso_utc(instante_parseado)}")

inicio = perf_counter()
sleep(0.25)
fim = perf_counter()

print(f"Duracao medida: {(fim - inicio) * 1000:.0f} ms")
