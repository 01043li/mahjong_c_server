# 上传C语言麻将项目到GitHub指南

## 项目信息
- **项目名称**: mahjong-c-server
- **项目描述**: C语言实现的麻将游戏服务器
- **技术栈**: C语言、TCP Socket、多线程、Makefile
- **状态**: 基础框架已完成，可编译运行

## 当前Git状态
```
提交历史：
b5f8e1c 添加完整的服务器实现
e7c3a9d 项目初始化：C语言麻将游戏完整框架
d41c8e2 初始提交：添加.gitignore文件
```

## 上传到GitHub步骤

### 步骤1: 登录GitHub
1. 访问 https://github.com
2. 使用你的GitHub账号登录

### 步骤2: 创建新仓库
1. 点击右上角 "+" → "New repository"
2. 填写仓库信息：
   - **Repository name**: `mahjong-c-server`
   - **Description**: `C语言实现的麻将游戏服务器，支持多人在线对战`
   - **Visibility**: Public (公开) 或 Private (私有)
   - **Initialize with README**: ✅ 勾选
   - **Add .gitignore**: 选择 "C"
   - **Choose a license**: 选择 "MIT License"
3. 点击 "Create repository"

### 步骤3: 获取仓库URL
创建成功后，你会看到类似这样的URL：
```
https://github.com/你的用户名/mahjong-c-server.git
```

### 步骤4: 添加远程仓库
在终端中执行：
```bash
cd /home/lihao/.openclaw/workspace/mahjong-c-project

# 添加远程仓库
git remote add origin https://github.com/你的用户名/mahjong-c-server.git

# 验证远程仓库
git remote -v
```

### 步骤5: 推送代码
```bash
# 推送代码到GitHub
git push -u origin main

# 如果遇到错误，可能是因为分支名不同
git push -u origin master
```

### 步骤6: 验证上传
1. 刷新GitHub仓库页面
2. 你应该能看到所有项目文件
3. 检查提交历史是否完整

## 备用方案：使用GitHub CLI

如果你安装了GitHub CLI，可以更简单：

```bash
# 安装GitHub CLI (如果未安装)
# Ubuntu/Debian: sudo apt install gh
# macOS: brew install gh

# 登录GitHub
gh auth login

# 创建仓库并推送
cd /home/lihao/.openclaw/workspace/mahjong-c-project
gh repo create mahjong-c-server --public --source=. --remote=origin --push
```

## 常见问题解决

### 问题1: 认证失败
**错误**: `remote: Invalid username or password`
**解决**:
1. 使用个人访问令牌代替密码
2. 生成令牌: GitHub → Settings → Developer settings → Personal access tokens
3. 选择权限: repo (全部仓库权限)
4. 推送时使用令牌作为密码

### 问题2: 分支名称冲突
**错误**: `error: failed to push some refs`
**解决**:
```bash
# 重命名本地分支
git branch -m master main

# 或者强制推送
git push -f origin master
```

### 问题3: 大文件上传失败
**解决**:
```bash
# 检查大文件
find . -type f -size +50M

# 从Git中移除大文件
git rm --cached 大文件路径
```

## 项目结构说明
```
mahjong-c-project/
├── Makefile              # 构建系统
├── README.md            # 项目说明
├── Git使用指南.md       # Git学习教程
├── 上传到GitHub指南.md  # 本文件
├── src/                 # 源代码
│   ├── server/         # 服务器代码
│   ├── common/         # 公共代码
│   └── test/           # 测试代码
├── include/            # 头文件
├── bin/               # 可执行文件
└── build/             # 编译中间文件
```

## 后续维护

### 日常开发流程
```bash
# 1. 拉取最新代码
git pull origin main

# 2. 创建新分支
git checkout -b feature/新功能

# 3. 开发完成后提交
git add .
git commit -m "添加新功能"

# 4. 推送到GitHub
git push origin feature/新功能

# 5. 在GitHub创建Pull Request
```

### 版本标签
```bash
# 创建版本标签
git tag v1.0.0
git push origin v1.0.0
```

## 学习资源
- Git官方文档: https://git-scm.com/doc
- GitHub官方指南: https://docs.github.com
- 廖雪峰Git教程: https://www.liaoxuefeng.com/wiki/896043488029600

## 联系方式
如果在上传过程中遇到问题，可以：
1. 查看Git错误信息
2. 搜索GitHub帮助文档
3. 在GitHub Issues中提问

---

**最后更新**: 2026-03-06 12:28 GMT+8
**项目状态**: 准备上传到GitHub