def media(valores):
    if not valores:
        return 0.0
    return sum(valores) / len(valores)

if __name__ == "__main__":
    leituras = [21.1, 22.4, 23.0, 21.8]
    print(f"Media Python: {media(leituras):.2f}")
