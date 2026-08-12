document.addEventListener("DOMContentLoaded", async () => {
  if (typeof mermaid === "undefined") {
    console.error("Mermaid não foi carregado; os diagramas não podem ser renderizados.");
    return;
  }

  mermaid.initialize({
    startOnLoad: false,
    securityLevel: "strict",
    theme: "neutral",
    flowchart: {
      htmlLabels: true,
      useMaxWidth: true,
    },
  });

  document.querySelectorAll(".mermaid").forEach((diagram) => {
    const code = diagram.querySelector(":scope > code");

    if (code) {
      diagram.textContent = code.textContent;
    }
  });

  await mermaid.run({
    querySelector: ".mermaid",
  });
});
