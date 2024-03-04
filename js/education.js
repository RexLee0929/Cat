document.addEventListener("DOMContentLoaded", function() {
    fetch('js/articles.json')
    .then(response => response.json())
    .then(data => {
        const articlesList = document.getElementById("articles-list");
        data.forEach(article => {
            if (article.path.endsWith('.md')) {
                fetch(article.path)
                .then(response => response.text())
                .then(text => {
                    const htmlContent = marked.parse ? marked.parse(text) : "Markdown parsing error";
                    articlesList.innerHTML += `<div class="article"><h2>${article.title}</h2><div>${htmlContent}</div></div>`;
                });
            } else {
                articlesList.innerHTML += `<div class="article"><h2><a href="${article.path}" target="_blank">${article.title}</a></h2></div>`;
            }
        });
    });
});
