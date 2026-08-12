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

  await mermaid.run({
    querySelector: ".mermaid",
  });
});
