document.getElementById('loginForm').addEventListener('submit', function(event) {
    event.preventDefault(); // 阻止表单默认提交行为

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // 简单的表单验证
    if(username === '' || password === '') {
        alert('用户名和密码不能为空！');
        return;
    }

    // 这里可以添加更多的逻辑，例如发送请求到服务器验证用户名和密码
    alert('登录成功！'); // 示例成功提示
});

function resetPassword() {
    // 假设这里弹出一个重置密码的对话框或跳转到重置密码的页面
    alert("重置密码功能待实现。");
}

document.getElementById('volunteerApplicationForm').onsubmit = function(event) {
    event.preventDefault();
    // 这里可以添加发送表单数据到服务器的代码
    alert('申请已提交，感谢您愿意成为志愿者！');
};

document.getElementById('volunteerApplicationForm').addEventListener('submit', function(event) {
    event.preventDefault();
    
    var birthdate = document.getElementsByName('birthdate')[0].value;
    // 假设birthdate的值为"1990-01-01"
    
    var parts = birthdate.split('-');
    var year = parts[0]; // "1990"
    var month = parts[1]; // "01"
    var day = parts[2]; // "01"
    
    console.log("年:", year);
    console.log("月:", month);
    console.log("日:", day);
    
    // 在这里可以添加将数据发送到服务器的代码
});



// 假设您从数据库中获取的数据是一个对象数组
var animalsData = [
    // 数据示例
    { 
        rescueDate: '2021-01-01', 
        healthStatus: '健康', 
        age: '2岁', 
        personality: '友好', 
        photo: 'path/to/photo1.jpg', 
        adopted: '是'
    },
    // ...更多动物数据
];

// 动态创建动物信息卡片并添加到页面
function displayAnimals(animals) {
    var animalsList = document.getElementById('animals-list');
    animalsList.innerHTML = ''; // 清空现有内容

    animals.forEach(function(animal) {
        var card = document.createElement('div');
        card.className = 'animal-card';
        
        card.innerHTML = `
            <img class="animal-photo" src="${animal.photo}" alt="动物照片">
            <div class="animal-info">
                <p>救助日期：${animal.rescueDate}</p>
                <p>健康状况：${animal.healthStatus}</p>
                <p>年龄：${animal.age}</p>
                <p>性格：${animal.personality}</p>
                <p>是否被领养：${animal.adopted}</p>
            </div>
        `;
        
        animalsList.appendChild(card);
    });
}

// 页面加载完成后执行
window.onload = function() {
    displayAnimals(animalsData);
};


// 假设您从后端获取了Markdown格式的文章内容
// 假设以下数组从后端获取或另一个JavaScript文件中定义
var articles = [
    { id: 1, title: '流浪猫的日常护理', summary: '本文介绍了如何照顾流浪猫，并提供了一些基本的护理技巧。' },
    { id: 2, title: '如何正确地喂养流浪猫', summary: '喂养流浪猫需要注意的事项，以及推荐的食物类型。' },
    // ...更多文章
];

// 动态创建文章列表并添加到页面中
function displayArticles(articles) {
    var articlesList = document.getElementById('articles-list');
    articles.forEach(function(article) {
        var articleDiv = document.createElement('div');
        articleDiv.className = 'article';
        
        var title = document.createElement('a');
        title.href = 'article-detail.html?id=' + article.id; // 假设每篇文章的详细页面URL格式
        title.className = 'article-title';
        title.textContent = article.title;
        title.onclick = function() {
            // 可以在这里添加重定向前的其他逻辑
            window.location.href = this.href;
        };
        
        var summary = document.createElement('div');
        summary.className = 'article-summary';
        summary.textContent = article.summary;
        
        articleDiv.appendChild(title);
        articleDiv.appendChild(summary);
        articlesList.appendChild(articleDiv);
    });
}

// 页面加载完成后执行
window.onload = function() {
    displayArticles(articles);
};

// 示例数据，实际情况中这些数据应从后端获取
const pendingTasks = [
    '救助任务1 - 待完成',
    '救助任务2 - 待完成',
    // 更多待完成任务...
];

const completedTasks = [
    '救助任务A - 已完成',
    '救助任务B - 已完成',
    // 更多已完成任务...
];

function displayTasks(taskList, containerId) {
    const container = document.getElementById(containerId);
    container.innerHTML = ''; // 清空现有内容

    taskList.forEach(task => {
        const taskDiv = document.createElement('div');
        taskDiv.classList.add('task');
        taskDiv.textContent = task;
        container.appendChild(taskDiv);
    });
}

window.onload = function() {
    displayTasks(pendingTasks, 'pending-tasks');
    displayTasks(completedTasks, 'completed-tasks');
};
